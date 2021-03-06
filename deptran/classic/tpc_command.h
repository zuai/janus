#pragma once
#include "../__dep__.h"
#include "../command.h"
#include "deptran/procedure.h"

namespace rococo {


class Procedure;
class TpcPrepareCommand : public ContainerCommand {
 public:
  TpcPrepareCommand() {
    type_ = MarshallDeputy::CMD_TPC_PREPARE;
    kind_ = MarshallDeputy::CMD_TPC_PREPARE;
  }
  txnid_t txn_id_ = 0;
  int32_t res_ = -1;
  vector<SimpleCommand> cmds_ = {};

  Marshal& ToMarshal(Marshal&) const override;
  Marshal& FromMarshal(Marshal&) override;
  virtual ContainerCommand& Execute() {verify(0);};
};

class TpcCommitCommand : public ContainerCommand {
 public:
  TpcCommitCommand() {
    type_ = MarshallDeputy::CMD_TPC_COMMIT;
    kind_ = MarshallDeputy::CMD_TPC_COMMIT;
  }
  txnid_t txn_id_ = 0;
  int res_ = -1;
  virtual Marshal& ToMarshal(Marshal&) const;
  virtual Marshal& FromMarshal(Marshal&);
  virtual ContainerCommand& Execute() {verify(0);};
};

} // namespace rococo