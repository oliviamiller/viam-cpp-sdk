#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <components/generic/generic.hpp>
#include <components/generic/server.hpp>


class MockGeneric : public Generic {
      public:
      std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(
      std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>command) override {
      return map;
    }

     std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<ProtoType>>> map;

};

std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<ProtoType>>> map() {
      ProtoType prototype = ProtoType(std::string("hello"));
      std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(prototype);
      std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {{std::string("test"), proto_ptr}};
      return std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);
}

std::shared_ptr<MockGeneric> get_mock_generic() {
    MockGeneric generic;
    generic.map = map();

    return std::make_shared<MockGeneric>(generic);
}

std::shared_ptr<MockGeneric> generic = get_mock_generic();

class MockStub : public viam::component::generic::v1::GenericService::StubInterface {
    public: 
    GenericServer server; 

    MockStub(): server(GenericServer (std::make_shared<SubtypeService>())) {
        this->server.sub_svc->add(std::string("generic"), get_mock_generic());
    }

    ::grpc::Status DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::viam::common::v1::DoCommandResponse* response) override {
        grpc::ServerContext* ctx;
        return server.DoCommand(ctx, &request, response);
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>> AsyncDoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>>(AsyncDoCommandRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>> PrepareAsyncDoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>>(PrepareAsyncDoCommandRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)>) override {
            return;
      };
      void DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) override{
        return;

      };
     public:
      friend class Stub;
      explicit async(MockStub* stub): stub_(stub) { }
      MockStub* stub() { return stub_; }
      MockStub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) override {
         return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) override {
          return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse, ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_, context, request);
    }

    const ::grpc::internal::RpcServiceMethod::RpcType type = ::grpc::internal::RpcServiceMethod::RpcType();
    const ::grpc::internal::RpcMethod rpcmethod_DoCommand_ = ::grpc::internal::RpcMethod("name", type);
  };
    