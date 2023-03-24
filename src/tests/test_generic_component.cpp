#define BOOST_TEST_MODULE test module test_generic_component
#include <common/v1/common.pb.h>

#include <boost/test/included/unit_test.hpp>
#include <components/generic/client.hpp>
#include <components/generic/generic.hpp>
#include <components/generic/server.hpp>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include "mocks/generic_mocks.cpp"

BOOST_AUTO_TEST_SUITE(generic_suite)

std::shared_ptr<MockGeneric> generic = get_mock_generic();

BOOST_AUTO_TEST_CASE(test_do) {
  std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(std::move(std::string("hello")));
  std::unordered_map<std::string, std::shared_ptr<ProtoType>> expected_map = {
      {std::string("test"), proto_ptr}};
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      expected = std::make_shared<
          std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(
          expected_map);

  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      command;
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      result_map = generic->do_command(command);

  std::shared_ptr<ProtoType> expected_pt = expected->at(std::string("test"));
  std::shared_ptr<ProtoType> result_pt = result_map->at(std::string("test"));

  BOOST_CHECK(*expected_pt == *result_pt);
}

MockStub *mock = new MockStub();
BOOST_AUTO_TEST_CASE(test_do_service) {
  viam::common::v1::DoCommandRequest req;
  viam::common::v1::DoCommandResponse resp;
  grpc::ClientContext ctx;

  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      command = std::make_shared<
          std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

  *req.mutable_command() = map_to_struct(command);
  *req.mutable_name() = "generic";

  std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(std::move(std::string("hello")));
  std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {
      {std::string("test"), proto_ptr}};

  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      expected_map = std::make_shared<
          std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);

  grpc::Status status = mock->DoCommand(&ctx, req, &resp);

  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      result_map = struct_to_map(resp.result());

  ProtoType expected_pt = *(expected_map->at(std::string("test")));
  ProtoType result_pt = *(result_map->at(std::string("test")));

  BOOST_CHECK(expected_pt == result_pt);
}

MockClient *client = new MockClient("generic");

BOOST_AUTO_TEST_CASE(test_do_client) {
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      command = std::make_shared<
          std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

  std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(std::move(std::string("hello")));
  std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {
      {std::string("test"), proto_ptr}};
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      expected_map = std::make_shared<
          std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);

  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
      result_map = client->do_command(command);

  ProtoType expected_pt = *(expected_map->at(std::string("test")));
  ProtoType result_pt = *(result_map->at(std::string("test")));

  BOOST_CHECK(expected_pt == result_pt);
}

BOOST_AUTO_TEST_SUITE_END()
