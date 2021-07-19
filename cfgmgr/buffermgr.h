#ifndef __BUFFMGR__
#define __BUFFMGR__

#include "dbconnector.h"
#include "producerstatetable.h"
#include "orch.h"

#include <map>
#include <string>

namespace swss {

#define INGRESS_LOSSLESS_PG_POOL_NAME "ingress_lossless_pool"
#define LOSSLESS_PGS "3-4"

/**
 * The buffer profile threshold mode can replace the mode define in buffer pool.
 * The threashold in pg_profile_lookup.ini may want to using static mode.
 * If the value be treate as dynamic mode will got problem.
 * Here check the threshold value to determin the mode.
 * Dynamic th normally will between -7 ~ 3, so here using -10 ~ 10 to checkit.
 * May be the better solution is to add mode to the pg_profile_lookup.ini
 * Here define the range to check
 */

#define BUFFER_POOL_DYNAMIC_THRESHOLD_CHECK_MIN -10
#define BUFFER_POOL_DYNAMIC_THRESHOLD_CHECK_MAX 10

typedef struct{
    std::string size;
    std::string xon;
    std::string xon_offset;
    std::string xoff;
    std::string threshold;
} pg_profile_t;

typedef std::map<std::string, pg_profile_t> speed_map_t;
typedef std::map<std::string, speed_map_t> pg_profile_lookup_t;

typedef std::map<std::string, std::string> port_cable_length_t;

class BufferMgr : public Orch
{
public:
    BufferMgr(DBConnector *cfgDb, DBConnector *stateDb, std::string pg_lookup_file, const std::vector<std::string> &tableNames);
    using Orch::doTask;

private:
    std::string m_platform;

    Table m_cfgPortTable;
    Table m_cfgCableLenTable;
    Table m_cfgBufferProfileTable;
    Table m_cfgBufferPgTable;
    Table m_cfgLosslessPgPoolTable;
    bool m_pgfile_processed;

    pg_profile_lookup_t m_pgProfileLookup;
    port_cable_length_t m_cableLenLookup;
    std::string getPgPoolMode();
    void readPgProfileLookupFile(std::string);
    task_process_status doCableTask(std::string port, std::string cable_length);
    task_process_status doPortTableUpdateTask(std::string port, std::string speed, bool admin_up);

    void doTask(Consumer &consumer);
};

}

#endif /* __BUFFMGR__ */
