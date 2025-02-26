/* Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "tensorflow/core/common_runtime/constant_folding.h"

#include "tensorflow/core/common_runtime/device_factory.h"
#include "tensorflow/core/common_runtime/device_mgr.h"
#include "tensorflow/core/framework/node_def_util.h"
#include "tensorflow/core/framework/tensor_testutil.h"
#include "tensorflow/core/framework/types.h"
#include "tensorflow/core/graph/node_builder.h"
#include "tensorflow/core/graph/testlib.h"
#include "tensorflow/core/lib/core/status_test_util.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/platform/test.h"
#include "tensorflow/core/public/session_options.h"
#include "tensorflow/core/public/tensor.h"
#include "tensorflow/core/public/tensor_shape.h"

namespace tensorflow {
namespace {

class ConstantFoldingTest : public ::testing::Test {
 protected:
  ConstantFoldingTest() { Reset(); }
  void Reset() { g_.reset(new Graph(OpRegistry::Global())); }

  template <typename T>
  Node* Constant(gtl::ArraySlice<T> values, TensorShape shape) {
    return test::graph::Constant(g_.get(), test::AsTensor(values, shape));
  }

  template <typename T>
  void ExpectNodeClose(const Node* n, gtl::ArraySlice<T> values,
                       TensorShape shape) {
    EXPECT_TRUE(n->IsConstant());
    const TensorProto* tensor_proto;
    TF_EXPECT_OK(GetNodeAttr(n->def(), "value", &tensor_proto));
    DataType dtype;
    TF_EXPECT_OK(GetNodeAttr(n->def(), "dtype", &dtype));
    Tensor t(dtype);
    EXPECT_TRUE(t.FromProto(*tensor_proto));
    test::ExpectClose(t, test::AsTensor(values, shape));
  }

// Construct the following graph
//    s1  s2
//    |    |
//    m1   m2
//    / \ / \
//   a   b   c
#define SIMPLE_GRAPH                                                         \
  Reset();                                                                   \
  Graph* g = g_.get();                                                       \
  Node* a = Constant<float>({1.0, 0.0, 0.0, 1.0}, {2, 2});                   \
  Node* b = Constant<float>({1.0, 2.0, 3.0, 4.0}, {2, 2});                   \
  Node* c = Constant<float>({0.0, 1.0, 1.0, 0.0}, {2, 2});                   \
  g->AddControlEdge(g->source_node(), a);                                    \
  g->AddControlEdge(g->source_node(), b);                                    \
  g->AddControlEdge(g->source_node(), c);                                    \
  Node* m1 = test::graph::Matmul(g, a, b, false, false);                     \
  Node* s1 = test::graph::Send(g_.get(), m1, "m1", "sender", 0, "receiver"); \
  Node* m2 = test::graph::Matmul(g, b, c, false, false);                     \
  Node* s2 = test::graph::Send(g_.get(), m2, "m2", "sender", 0, "receiver"); \
  g->AddControlEdge(s1, g->sink_node());                                     \
  g->AddControlEdge(s2, g->sink_node());

  std::unique_ptr<Graph> g_;
};

TEST_F(ConstantFoldingTest, Basic) {
  SIMPLE_GRAPH;
  EXPECT_TRUE(DoConstantFolding(ConstantFoldingOptions{}, g));

  // Nodes s1 and s2 now should now have a constant input
  EXPECT_EQ(1, s1->num_inputs());
  ExpectNodeClose<float>(*(s1->in_nodes().begin()), {1.0, 2.0, 3.0, 4.0},
                         {2, 2});
  EXPECT_EQ(1, s2->num_inputs());
  ExpectNodeClose<float>(*(s2->in_nodes().begin()), {2.0, 1.0, 4.0, 3.0},
                         {2, 2});
}

TEST_F(ConstantFoldingTest, ConsiderFunction) {
  SIMPLE_GRAPH;
  ConstantFoldingOptions opts;
  // Do not allow constant folding of m2
  opts.consider = [m2](const Node* n) { return m2 != n; };
  EXPECT_TRUE(DoConstantFolding(opts, g));

  // Node s1 now should now have a constant input
  EXPECT_EQ(1, s1->num_inputs());
  ExpectNodeClose<float>(*(s1->in_nodes().begin()), {1.0, 2.0, 3.0, 4.0},
                         {2, 2});
  // s2's input should still be m2
  EXPECT_EQ(1, s2->num_inputs());
  EXPECT_EQ(*(s2->in_nodes().begin()), m2);
}

}  // namespace
}  // namespace tensorflow
