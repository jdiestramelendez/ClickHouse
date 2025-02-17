#pragma once

#include "config.h"

#if USE_PARQUET && USE_DELTA_KERNEL_RS

#include <Interpreters/Context_fwd.h>
#include <Core/Types.h>
#include <Storages/ObjectStorage/StorageObjectStorage.h>
#include <Storages/ObjectStorage/StorageObjectStorageSource.h>
#include <Storages/ObjectStorage/DataLakes/IDataLakeMetadata.h>
#include <Disks/ObjectStorages/IObjectStorage.h>

namespace DeltaLake
{
class TableSnapshot;
}

namespace DB
{

class DeltaLakeMetadata final : public IDataLakeMetadata
{
public:
    using ConfigurationObserverPtr = StorageObjectStorage::ConfigurationObserverPtr;
    static constexpr auto name = "DeltaLake";

    DeltaLakeMetadata(ObjectStoragePtr object_storage_, ConfigurationObserverPtr configuration_, ContextPtr context_);

    Strings getDataFiles() const override;

    NamesAndTypesList getTableSchema() const override;

    NamesAndTypesList getReadSchema() const override;

    const std::unordered_map<String, String> & getColumnNameToPhysicalNameMapping() const override { return noop; }

    bool operator ==(const IDataLakeMetadata &) const override
    {
        return false;
        // const auto * deltalake_metadata = dynamic_cast<const DeltaLakeMetadata *>(&other);
        // return deltalake_metadata
        //     && !data_files.empty() && !deltalake_metadata->data_files.empty()
        //     && data_files == deltalake_metadata->data_files;
    }

    static DataLakeMetadataPtr create(ObjectStoragePtr object_storage, ConfigurationObserverPtr configuration, ContextPtr local_context)
    {
        return std::make_unique<DeltaLakeMetadata>(object_storage, configuration, local_context);
    }

    bool supportsFileIterator() const override { return true; }

    ObjectIterator iterate() const override;

private:
    const LoggerPtr log;
    const std::shared_ptr<DeltaLake::TableSnapshot> table_snapshot;
    std::unordered_map<String, String> noop; /// Not needed for delta lake.
};

}

#endif
