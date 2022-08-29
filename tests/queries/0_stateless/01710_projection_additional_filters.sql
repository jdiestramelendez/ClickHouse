DROP TABLE IF EXISTS t;

CREATE TABLE t(a UInt32, b UInt32) ENGINE = MergeTree PARTITION BY a ORDER BY a;

INSERT INTO t SELECT number % 10, number FROM numbers(10000);

SELECT count(), min(a), max(a) FROM t SETTINGS additional_table_filters = {'t' : '0'};

DROP TABLE t;
