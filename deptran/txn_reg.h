#pragma once
#include "__dep__.h"
#include "constants.h"
namespace rococo {

class RequestHeader;
class Executor;

typedef std::map<
    int,
    mdb::Row *> row_map_t;

class DTxn;
class TxnChopper;
class TxnRequest;


typedef std::function<void(Executor* exec,
                           DTxn *dtxn,
                           const RequestHeader &header,
                           map<int32_t, Value> &input,
                           rrr::i32 *res,
                           map<int32_t, Value> &output //,
//    rrr::i32 *output_size//,
//    row_map_t *row_map
)> TxnHandler;

typedef enum {
  DF_REAL,
  DF_NO,
  DF_FAKE
} defer_t;

typedef struct {
  TxnHandler txn_handler;
  defer_t defer;
} txn_handler_defer_pair_t;

typedef std::function<bool(TxnChopper*,
                           map<int32_t, Value>&)>
    PieceCallbackHandler;

//class PieceCallbackMap {
// public:
//  map<std::pair<txntype_t, innid_t>, PieceCallbackHandler> callbacks_ = {};
//  virtual PieceCallbackHandler& Get(txntype_t txn_type, innid_t inn_id) {
//    return callbacks_.at(std::make_pair(txn_type, inn_id));
//  }
//
//  virtual void Set(txntype_t txn_type, innid_t inn_id,
//                   PieceCallbackHandler& handler) {
//    callbacks_[std::make_pair(txn_type, inn_id)] = handler;
//  }
//};


/**
* This class holds all the hard-coded transactions pieces.
*/
class TxnRegistry {
 public:

  TxnRegistry() : callbacks_() {};

  void reg(base::i32 t_type, base::i32 p_type,
           defer_t defer, const TxnHandler &txn_handler) {
    auto func_key = std::make_pair(t_type, p_type);
    auto it = all_.find(func_key);
    verify(it == all_.end());
    all_[func_key] = (txn_handler_defer_pair_t) {txn_handler, defer};
  }

  txn_handler_defer_pair_t get(const base::i32 t_type,
                               const base::i32 p_type) {
    auto it = all_.find(std::make_pair(t_type, p_type));
    // Log::debug("t_type: %d, p_type: %d", t_type, p_type);
    verify(it != all_.end());
    return it->second;
  }
  txn_handler_defer_pair_t get(const RequestHeader &req_hdr);

 public:
  // prevent instance creation
//  TxnRegistry() { }
  map<std::pair<base::i32, base::i32>, txn_handler_defer_pair_t> all_ = {};
  map<std::pair<txntype_t, innid_t>, PieceCallbackHandler> callbacks_ = {};
  map<txntype_t, std::function<void(TxnChopper* ch, TxnRequest& req)> > init_ = {};
  map<txntype_t, std::function<void(TxnChopper* ch)>> retry_ = {};

//  PieceCallbackMap callbacks_;
//    static map<std::pair<base::i32, base::i32>, LockSetOracle> lck_oracle_;

};


} // namespace rococo