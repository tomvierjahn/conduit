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

#include "conduit.hpp"

#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace conduit;

namespace testing {

conduit::Node AnyNode() {
  conduit::Node node;
  node["A/B/C"] = 3.1415;
  node["A/B/D"] = 4.124;
  node["A/E"] = 42.0;
  return node;
}

struct Storage {
  std::string schema;
  std::vector<uint8> data;
  Storage(const Node &node) {
    Schema compact_schema;
    node.schema().compact_to(compact_schema);
    schema = compact_schema.to_json();

    node.serialize(data);
  }
};

} // namespace testing

//-----------------------------------------------------------------------------
TEST(conduit_node_copy_external, copy) {
  testing::Storage storage(testing::AnyNode());

  const bool external = true;
  Node external_node(storage.schema, storage.data.data(), external);
  const std::string original_json(external_node.to_json());

  Node copied_node(external_node);
  const std::string original_json_copied(copied_node.to_json());

  storage.data[0] = ~storage.data[0];
  storage.data[7] = ~storage.data[7];

  EXPECT_NE(original_json, external_node.to_json());
  EXPECT_NE(original_json_copied, copied_node.to_json());

  EXPECT_EQ(external_node.to_json(), copied_node.to_json());
}

//-----------------------------------------------------------------------------
TEST(conduit_node_copy_external, assign) {
  testing::Storage storage(testing::AnyNode());

  const bool external = true;
  Node external_node(storage.schema, storage.data.data(), external);
  const std::string original_json(external_node.to_json());

  Node assigned_node;
  assigned_node = external_node;
  const std::string original_json_assigned(assigned_node.to_json());

  storage.data[0] = ~storage.data[0];
  storage.data[7] = ~storage.data[7];

  EXPECT_NE(original_json, external_node.to_json());
  EXPECT_NE(original_json_assigned, assigned_node.to_json());

  EXPECT_EQ(external_node.to_json(), assigned_node.to_json());
}
