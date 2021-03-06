// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef  TERA_SCHEMA_IMPL_H_
#define  TERA_SCHEMA_IMPL_H_

#include <string>

#include "tera/proto/table_meta.pb.h"
#include "tera/sdk/tera.h"

namespace tera {

/// 列族描述
class CFDescImpl : public ColumnFamilyDescriptor {
public:
    /// 列族名字仅允许使用字母、数字和下划线构造, 长度不超过256
    CFDescImpl(const std::string& cf_name, int32_t id, const std::string& lg_name);
    /// id
    int32_t Id() const;
    const std::string& Name() const;

    const std::string& LocalityGroup() const;

    /// 历史版本保留时间, 不设置时为0， 表示无限大永久保存
    void SetTimeToLive(int32_t ttl);

    int32_t TimeToLive() const;

    /// 在TTL内,最多存储的版本数
    void SetMaxVersions(int32_t max_versions);

    int32_t MaxVersions() const;

    /// 最少存储的版本数,即使超出TTL,也至少保留min_versions个版本
    void SetMinVersions(int32_t min_versions);

    int32_t MinVersions() const;

    /// 存储限额, MBytes
    void SetDiskQuota(int64_t quota);

    int64_t DiskQuota() const;

    /// ACL
    void SetAcl(ACL acl);

    ACL Acl() const;

    void SetType(const std::string& type);

    const std::string& Type() const;

private:
    int32_t _id;
    std::string _name;
    std::string _lg_name;
    int32_t _max_versions;
    int32_t _min_versions;
    int32_t _ttl;
    int64_t _acl;
    int32_t _owner;
    int32_t _disk_quota;
    std::string _type;
};

/// 局部性群组描述
class LGDescImpl : public LocalityGroupDescriptor {
public:
    /// 局部性群组名字仅允许使用字母、数字和下划线构造,长度不超过256
    LGDescImpl(const std::string& lg_name, int32_t id);

    /// Id read only
    int32_t Id() const;

    /// Name read only
    const std::string& Name() const;

    /// Compress type
    void SetCompress(CompressType type);

    CompressType Compress() const;

    /// Block size
    void SetBlockSize(int block_size);

    int BlockSize() const;

    /// Store type
    void SetStore(StoreType type);

    StoreType Store() const;

    /// Bloomfilter
    void SetUseBloomfilter(bool use_bloomfilter);

    bool UseBloomfilter() const;

    /// Memtable On Leveldb (disable/enable)
    bool UseMemtableOnLeveldb() const;

    void SetUseMemtableOnLeveldb(bool use_mem_ldb);

    /// Memtable-LDB WriteBuffer Size
    int32_t MemtableLdbWriteBufferSize() const;

    void SetMemtableLdbWriterBufferSize(int32_t buffer_size);

    /// Memtable-LDB Block Size
    int32_t MemtableLdbBlockSize() const;

    void SetMemtableLdbBlockSize(int32_t block_size);

private:
    int32_t         _id;
    std::string     _name;
    CompressType    _compress_type;
    StoreType       _store_type;
    int             _block_size;
    bool            _use_bloomfilter;
    bool            _use_memtable_on_leveldb;
    int32_t         _memtable_ldb_write_buffer_size;
    int32_t         _memtable_ldb_block_size;
};

/// 表描述符.
class TableDescImpl {
public:
    /// 表格名字仅允许使用字母、数字和下划线构造,长度不超过256
    TableDescImpl(const std::string& tb_name, bool is_kv);
    ~TableDescImpl();
    std::string TableName() const;
    /// 设置为kv表（无列），建表完成后无法改变
    void SetKvOnly();
    /// 增加一个localitygroup
    LocalityGroupDescriptor* AddLocalityGroup(const std::string& lg_name);
    /// 获取默认localitygroup，仅用于kv表
    LocalityGroupDescriptor* DefaultLocalityGroup();
    /// 删除一个localitygroup
    bool RemoveLocalityGroup(const std::string& lg_name);
    /// 获取localitygroup
    const LocalityGroupDescriptor* LocalityGroup(int32_t id) const;
    const LocalityGroupDescriptor* LocalityGroup(const std::string& lg_name) const;
    /// 获取localitygroup数量
    int32_t LocalityGroupNum() const;
    /// 增加一个columnfamily
    ColumnFamilyDescriptor* AddColumnFamily(const std::string& cf_name,
                                            const std::string& lg_name);
    ColumnFamilyDescriptor* DefaultColumnFamily();
    /// 删除一个columnfamily
    void RemoveColumnFamily(const std::string& cf_name);
    /// 获取所有的colmnfamily
    const ColumnFamilyDescriptor* ColumnFamily(int32_t id) const;
    const ColumnFamilyDescriptor* ColumnFamily(const std::string& cf_name) const;
    int32_t ColumnFamilyNum() const;

    /// Raw Key Mode
    void SetRawKey(RawKeyType type);
    RawKeyType RawKey() const;

    void SetSplitSize(int64_t size);
    int64_t SplitSize() const;

    void SetMergeSize(int64_t size);
    int64_t MergeSize() const;

    /// 插入snapshot
    int32_t AddSnapshot(uint64_t snapshot);
    /// 获取snapshot
    uint64_t Snapshot(int32_t id) const;
    /// Snapshot数量
    int32_t SnapshotNum() const;
    /// 是否为kv表
    bool IsKv() const;
private:
    typedef std::map<std::string, LGDescImpl*> LGMap;
    typedef std::map<std::string, CFDescImpl*> CFMap;
    std::string     _name;
    bool            _kv_only;
    LGMap           _lg_map;
    std::vector<LGDescImpl*> _lgs;
    CFMap           _cf_map;
    std::vector<CFDescImpl*> _cfs;
    int32_t         _next_lg_id;
    int32_t         _next_cf_id;
    std::vector<uint64_t> _snapshots;
    static const std::string DEFAULT_LG_NAME;
    static const std::string DEFAULT_CF_NAME;
    RawKeyType      _raw_key_type;
    int64_t         _split_size;
    int64_t         _merge_size;
};

} // namespace tera

#endif  //TERA_SCHEMA_IMPL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
