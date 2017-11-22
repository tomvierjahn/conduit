//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2014-2017, Lawrence Livermore National Security, LLC.
//
// Produced at the Lawrence Livermore National Laboratory
//
// LLNL-CODE-666778
//
// All rights reserved.
//
// This file is part of Conduit.
//
// For details, see: http://software.llnl.gov/conduit/.
//
// Please also read conduit/LICENSE
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the disclaimer below.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the disclaimer (as noted below) in the
//   documentation and/or other materials provided with the distribution.
//
// * Neither the name of the LLNS/LLNL nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
// LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//-----------------------------------------------------------------------------
///
/// file: conduit_serialize.cpp
///
//-----------------------------------------------------------------------------

#include "conduit.hpp"

#include "gtest/gtest.h"
#include <iostream>
using namespace conduit;

namespace {

void serialize_const_ref(const Node &node, std::string *schema,
                         std::vector<uint8> *bytes) {
  Schema compact_schema;
  node.schema().compact_to(compact_schema);
  const std::string compact_schema_json(compact_schema.to_json());
  schema->assign(compact_schema_json.begin(), compact_schema_json.end());

  // intermediate vector not required in general, but my use case requires it
  std::vector<conduit::uint8> data;
  node.serialize(data);
  bytes->assign(data.begin(), data.end());
}

void serialize_value(Node node, std::string *schema,
                     std::vector<uint8> *bytes) {
  Schema compact_schema;
  node.schema().compact_to(compact_schema);
  const std::string compact_schema_json(compact_schema.to_json());
  schema->assign(compact_schema_json.begin(), compact_schema_json.end());

  // intermediate vector not required in general, but my use case requires it
  std::vector<conduit::uint8> data;
  node.serialize(data);
  bytes->assign(data.begin(), data.end());
}

} // namespace

//-----------------------------------------------------------------------------
TEST(conduit_serialize_read_multiple_times, const_ref) {
  Node node;
  node["a/b/c"] = 3.1415;
  node["a/b/d"] = 4.123;
  node["a/e"] = 42.0;

  std::string schema;
  std::vector<uint8> bytes;

  serialize_const_ref(node, &schema, &bytes);

  Node second_node;
  second_node.set_data_using_schema(Schema(schema), bytes.data());
  EXPECT_EQ(node["a/b/c"].as_double(), second_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), second_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), second_node["a/e"].as_double());

  serialize_const_ref(second_node, &schema, &bytes);

  Node third_node;
  third_node.set_data_using_schema(Schema(schema), bytes.data());

  EXPECT_EQ(node["a/b/c"].as_double(), third_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), third_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), third_node["a/e"].as_double());
}

//-----------------------------------------------------------------------------
TEST(conduit_serialize_read_multiple_times, value) {
  Node node;
  node["a/b/c"] = 3.1415;
  node["a/b/d"] = 4.123;
  node["a/e"] = 42.0;

  std::string schema;
  std::vector<uint8> bytes;

  serialize_value(node, &schema, &bytes);

  Node second_node;
  second_node.set_data_using_schema(Schema(schema), bytes.data());
  EXPECT_EQ(node["a/b/c"].as_double(), second_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), second_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), second_node["a/e"].as_double());

  serialize_value(second_node, &schema, &bytes);

  Node third_node;
  third_node.set_data_using_schema(Schema(schema), bytes.data());
  EXPECT_EQ(node["a/b/c"].as_double(), third_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), third_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), third_node["a/e"].as_double());
}

//-----------------------------------------------------------------------------
TEST(conduit_serialize_read_multiple_times, const_ref__constructor) {
  Node node;
  node["a/b/c"] = 3.1415;
  node["a/b/d"] = 4.123;
  node["a/e"] = 42.0;

  std::string schema;
  std::vector<uint8> bytes;

  serialize_const_ref(node, &schema, &bytes);

  Node second_node(schema, bytes.data(), false);
  EXPECT_EQ(node["a/b/c"].as_double(), second_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), second_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), second_node["a/e"].as_double());

  serialize_const_ref(second_node, &schema, &bytes);

  Node third_node(schema, bytes.data(), false);

  EXPECT_EQ(node["a/b/c"].as_double(), third_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), third_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), third_node["a/e"].as_double());
}

//-----------------------------------------------------------------------------
TEST(conduit_serialize_read_multiple_times, value__constructor) {
  Node node;
  node["a/b/c"] = 3.1415;
  node["a/b/d"] = 4.123;
  node["a/e"] = 42.0;

  std::string schema;
  std::vector<uint8> bytes;

  serialize_value(node, &schema, &bytes);

  Node second_node(schema, bytes.data(), false);
  EXPECT_EQ(node["a/b/c"].as_double(), second_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), second_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), second_node["a/e"].as_double());

  serialize_value(second_node, &schema, &bytes);

  Node third_node(schema, bytes.data(), false);

  EXPECT_EQ(node["a/b/c"].as_double(), third_node["a/b/c"].as_double());
  EXPECT_EQ(node["a/b/d"].as_double(), third_node["a/b/d"].as_double());
  EXPECT_EQ(node["a/e"].as_double(), third_node["a/e"].as_double());
}
