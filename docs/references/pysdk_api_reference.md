---
sidebar_position: 2
slug: /pysdk_api_reference
---
# Python API Reference

A complete reference for Infinity's Python APIs.

---

## SCHEMA MANAGEMENT

---

### connect

```python
# Connect to the Infinity server and get an Infinity object
infinity.connect(uri)
```

Connects to the Infinity server, and gets an Infinity object.

:::tip NOTE
You must have an Infinity object ready to perform database-specific operations.
:::

#### Parameters

##### uri: *Required*

The `uri` here is a `NetworkAddress` object:  

`NetworkAddress`: Used in client-server mode, when you have deployed Infinity as a separate server and wish to connect to it remotely. A `NetworkAddress` object comprises two fields:

- `"<SERVER_IP_ADDRESS>"`: `str` - The IP address of the Infinity server.  
- `<PORT>`: `int` - The SDK port number on which the Infinity server listens. Defaults to `23817`.

:::tip IMPORTANT
When setting `uri` as `NetworkAddress`, ensure you:
- Install the Infinity SDK: `pip install infinity==<VERSION>`
- Import the `infinity` module: `import infinity`.
:::

:::caution IMPORTANT
When connecting to Infinity in client-server mode, ensure that the client version *exactly* matches the server version. For example:

| **Client version** | **Server version** |
|--------------------|--------------------|
| v0.1.0             | v0.1.0             |
| v0.1.1             | v0.1.1             |
| v0.2.0             | v0.2.0             |
| v0.2.1             | v0.2.1             |
| v0.3.0             | v0.3.0             |
| v0.4.0             | v0.4.0             |
| v0.5.0-dev2        | v0.5.0-dev2        |

If the versions do not match, please update your client or server to ensure compatibility.

In client-server mode, also ensure that your server version matches the version specified in your configuration file. Here, the matching rule is less strict than an exact match:

- The major and minor versions *must* be identical.
- The patch version may differ.

This allows for bug fixes without requiring changes to the configuration file.

| **Configuration version** | **Compatible server version** |
|---------------------------|-------------------------------|
| v0.1.0                    | v0.1.0, v0.1.1                |
| v0.2.0                    | v0.2.0, v0.2.1                |

:::

#### Returns

- Success: An `infinity.remote_thrift.infinity.RemoteThriftInfinityConnection` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` - A non-zero value indicating a specific error condition.
  - `error_msg`: `str` - A message providing additional details about the error.

#### Examples

##### Connect to Infinity in client-server mode

If you have deployed Infinity as a separate server and installed the Infinity client via `pip install infinity==<VERSION>`, you can connect to it via its IP address. If your Infinity is running on your local machine, you can also use `infinity.common.LOCAL_HOST` to replace `"<SERVER_IP_ADDRESS>"` in the following code snippet.

```python
import infinity
# If Infinity is deployed on the local machine, use infinity.LOCAL_HOST to replace <SERVER_IP_ADDRESS>
infinity_object = infinity.connect(infinity.NetworkAddress("192.168.1.101", 23817)) 
```

---

### disconnect

```python
infinity_object.disconnect()
```

Disconnects the client from the Infinity server in client-server mode, or destructs the Infinity object and releases all associated resources when Infinity is imported as a Python module.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
infinity_object.disconnect()
```

---

### create_database

```python
infinity_object.create_database(db_name, conflict_type = ConflictType.Error, comment = None)
```

Creates a database with a specified name.

#### Parameters

##### db_name: `str`, *Required*

A non-empty string indicating the name of the database, which must adhere to the following requirements:

- Permitted characters include:
  - English letters (a-z, A-Z)
  - Digits (0-9)
  - "_" (underscore)
- Must begin with an English letter or underscore.
- Maximum 65,535 characters.
- Case-insensitive.

##### conflict_type: `ConflictType`, *Optional*

- `Error`: Raise an error if a database with the same name exists.
- `Ignore`: Ignore the database creation request and keep the existing database with the same name.

:::tip NOTE
You may want to import the `infinity.common` package to set `ConflictType`:

```python
from infinity.common import ConflictType
```
:::

:::tip NOTE
If `ConflictType` is not set, it defaults to `Error`.
:::

##### comment: `str`, *Optional*

Additional comment for the database to create.

#### Returns

- Success: An `infinity.remote_thrift.db.RemoteDatabase` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` - A non-zero value indicating a specific error condition.
  - `error_msg`: `str` - A message providing additional details about the error.

#### Examples

```python
# Create a database named 'my_database':
# If the specified database already exists, raise an error. 
infinity_object.create_database("my_database")
```

```python
# Create a database named 'my_database':
# If the specified database already exists, raise an error (same as above). 
infinity_object.create_database("my_database", infinity.common.ConflictType.Error, comment="Database One")
```

```python
from infinity.common import ConflictType
# Create a database named 'my_database':
# If the specified database already exists, silently ignore the operation and proceed. 
infinity_object.create_database("my_database", ConflictType.Ignore, comment="Database One")
```

---

### drop_database

```python
infinity_object.drop_database(db_name, conflict_type = ConflictType.Error)
```

Deletes a database by its name.

#### Parameters

##### db_name: `str`, *Required*

A non-empty string indicating the name of the database to delete.

##### conflict_type: `ConflictType`, *Optional*

- `Error`: Raise an error if the specified database does not exist.
- `Ignore`: Ignore the operation and proceed regardless, if the specified database does not exist.

:::tip NOTE
You may want to import the `infinity.common` package to set `ConflictType`:

```python
from infinity.common import ConflictType
```

:::

:::tip NOTE
If `ConflictType` is not set, it defaults to `Error`.
:::

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
# Delete a database named 'my_database':
# If the specified database does not exist, raise an error. 
infinity_object.drop_database("my_database")
```

```python
# Delete a database named 'my_database':
# If the specified database does not exist, raise an error (same as above). 
infinity_object.drop_database("my_database", infinity.common.ConflictType.Error)
```

```python
from infinity.common import ConflictType
# Delete a database named 'my_database':
# If the specified database does not exist, silently ignore the operation and proceed.
infinity_object.drop_database("my_database", ConflictType.Ignore)
```

---

### list_databases

```python
infinity_object.list_databases()
```

Retrieves a list of all available databases within the Infinity system.

#### Returns

A structure containing the following attributes:

- `db_names`: `list[str]` A list of strings indicating the names of all available databases.
- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
res = infinity_object.list_databases() 
print(res.db_names) # ['my_database', 'database_1']
```

---

### show_database

```python
infinity_object.show_database(database_name)
```

Shows detailed information about a database.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
res = infinity_object.show_database('my_database')
```

---

### get_database

```python
infinity_object.get_database(database_name)
```

Retrieves a database object by its name.

#### Parameters

##### db_name: `str`, *Required*

A non-empty string indicating the name of the database to retrieve.

#### Returns

- Success: An `infinity.remote_thrift.db.RemoteDatabase` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` - A non-zero value indicating a specific error condition.
  - `error_msg`: `str` - A message providing additional details about the error.

#### Examples

```python
db_object = infinity_object.get_database("my_database")
```

---

### create_table

```python
db_object.create_table(table_name, columns_definition, conflict_type = ConflictType.Error)
```

Creates a table with a specified name and defined columns.

:::tip NOTE
Call `create_database()` or `get_database()` to get a database object for all table-specific operations.
:::

#### Parameters

##### table_name: `str`, *Required*

A non-empty string indicating the name of the table, which must adhere to the following requirements:

- Permitted characters include:
  - English letters (a-z, A-Z)
  - Digits (0-9)
  - "_" (underscore)
- Must begin with an English letter or underscore.
- Maximum 65,535 characters.
- Case-insensitive.

##### columns_definition: `dict[str, dict[str, Any]]`, *Required*

Definitions for all table columns as a dictionary. Each key in the dictionary is a column name (a `str` following the same rule as `table_name`), with a corresponding 'value' dictionary defining the column's data type and default value information in key-value pairs:

- **Data type** (`"type"`)  
  The data type of the column.  
  - Numeric:  
    - `"int8"`
    - `"int16"`
    - `"int"`/`"int32"`/`"integer"`
    - `"int64"`
    - `"float"`/`"float32"`
    - `"double"`/`"float64"`
    - `"float16"`
    - `"bfloat16"`
  - String: `"varchar"`
  - Dense vector: e.g., `"vector,128,float"`
    - `vector`: The column is a dense vector column.
    - The second item in the string: The dimension of dense vector.
    - The third item in the string: The element type of dense vector. Can be:
      - `"int8"`
      - `"int16"`
      - `"int"`/`"int32"`/`"integer"`
      - `"int64"`
      - `"float"`/`"float32"`
      - `"double"`/`"float64"`
      - `"float16"`
      - `"bfloat16"`
  - Sparse vector: e.g., `"sparse,128,float,int"`
    - `sparse`: The column is a sparse vector column.
    - The second item in the string: The dimension of sparse vector.
    - The third item in the string: The element type of sparse vector. Can be:
      - `"int8"`
      - `"int16"`
      - `"int"`/`"int32"`/`"integer"`
      - `"int64"`
      - `"float"`/`"float32"`
      - `"double"`/`"float64"`
      - `"float16"`
      - `"bfloat16"`
    - The fourth item in the string: The data type of sparse vector indices. Can be:
      - `int8`
      - `int16`
      - `int`/`int32`/`integer`
      - `int64`
  - Tensor vector: e.g., `"tensor,4,float"`
    - `tensor`: The column is a tensor column.
    - The second item in the string: The dimension of each vector unit in a tensor.
    - The third item in the string: The element type of tensors. Can be:
      - `"int8"`
      - `"int16"`
      - `"int"`/`"int32"`/`"integer"`
      - `"int64"`
      - `"float"`/`"float32"`
      - `"double"`/`"float64"`
      - `"float16"`
      - `"bfloat16"`
  - Tensor array: e.g., `"tensorarray,6,float"`
    - `tensorarray`: The column is a tensor-array column.
    - The second item in the string: The dimension of each vector unit in a tensor array.
    - The third item in the string: The element type of tensor arrays. Can be:
      - `"int8"`
      - `"int16"`
      - `"int"`/`"int32"`/`"integer"`
      - `"int64"`
      - `"float"`/`"float32"`
      - `"double"`/`"float64"`
      - `"float16"`
      - `"bfloat16"`
  - Multivector: e.g., `"multivector,128,float"`
    - `multivector`: The column is a multi-vector column.
    - The second item in the string: The dimension of each vector unit in a multivector.
    - The third item in the string: The element type of multi-vectors. Can be:
      - `"int8"`
      - `"int16"`
      - `"int"`/`"int32"`/`"integer"`
      - `"int64"`
      - `"float"`/`"float32"`
      - `"double"`/`"float64"`
      - `"float16"`
      - `"bfloat16"`
  - Array: e.g., `"array,varchar"`, `"array,array,varchar"`
    - `array`: The column is an array column.
    - followed by the element type of the array. Can be recursive.
    - for example, `"array,varchar"` is a one-dimensional array of strings, and `"array,array,varchar"` is a two-dimensional array of strings.
- **Default value** (`"default"`)  
  The default value for unspecified cells in that column.  

##### conflict_type: `ConflictType`, *Optional*

- `Error`: Raise an error if a table with the same name exists.
- `Ignore`: Ignore the table creation request and keep the existing table with the same name.

:::tip NOTE
You may want to import the `infinity.common` package to set `ConflictType`:

```python
from infinity.common import ConflictType
```

:::

:::tip NOTE
If `ConflictType` is not set, it defaults to `Error`.
:::

#### Returns

- Success: An `infinity.remote_infinity.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`:
  - `error_code`: `int` - A non-zero value indicating a specific error condition.
  - `error_msg`: `str` - A message providing additional details about the error.

#### Examples

##### Create a table with an integer column only

```python
# The `create_table`method supports creating integer columns in the following data types:
# - int8
# - int16
# - int/int32/integer
# - int64
db_object.create_table("my_table", {"c1": {"type": "int", "default": 1}})
```

##### Create a table with a float column only

```python
# The `create_table`method supports creating float columns in the following data types:
# - float/float32
# - double/float64
# - float16
# - bfloat16
db_object.create_table("my_table", {"c1": {"type": "float64"}})
```

##### Create a table with a string column only

```python
db_object.create_table("my_table", {"c1": {"type": "varchar"}})
```

##### Create a table with a bool column only

```python
db_object.create_table("my_table", {"c1": {"type": "bool"}})
```

##### Create a table with a vector column only

:::tip NOTE
You can build a HNSW index on the vector column to speed up the match_dense search.
:::

```python
# Create a table with a vector column only:  
# - `vector`: The column is a vector column
# - `128`: The vector dimension
# - `float`: The primitive data type of the vectors. Can be `float`/`float32`, `float16`, `bfloat16`, `uint8` or `int8`
db_object.create_table("my_table", {"c1": {"type": "vector,128,float"}})

```

##### Create a table with a multi-vector column only

:::tip NOTE
You can build an HNSW index on the multi-vector column to accelerate match_dense search.
:::

```python
# Create a table with a multi-vector column only:  
# - `multivector`: The column is a multi-vector column
# - `128`: The basic vector dimension
# - `float`: The primitive data type of the basic vectors. Can be `float`/`float32`, `float16`, `bfloat16`, `uint8` or `int8`
db_object.create_table("my_table", {"c1": {"type": "multivector,128,float"}})

```

##### Create a table with a sparse vector column only

:::tip NOTE
You can build a BMP index on the sparse vector column to speed up the match_sparse search.
:::

```python
from infinity.common import ConflictType
# Create a table with a vector column only:  
# - `sparse`: The column is a sparse vector column
# - `128`: The sparse vector dimension
# - `float`: The primitive data type of the sparse vectors. Can be `float`/`float32` or `double`/`float64`
# - `int`: The data type of the sparse vector indices. Can be `int8`, `int16`, `int`/`int32`/`integer`, or `int64`
db_object.create_table("my_table", {"c1": {"type": "sparse,128,float,int"}}, ConflictType.Error)
```

##### Create a table with a tensor column only

```python
from infinity.common import ConflictType
# Create a table with a tensor column only:  
# - `tensor`: The column is a tensor column
# - `4`: Dimension of each vector unit in the tensor
# - `float`: The primitive data type of the tensors. Can be `float`/`float32`, `float16`, `bfloat16` or `bit`
db_object.create_table("my_table", {"c1": {"type": "tensor,4,float"}}, ConflictType.Ignore)
```

##### Create a table with a tensor array column only

```python
from infinity.common import ConflictType
# Create a table with a tensor array column only:  
# - `tensorarray`: The column is a tensor array column
# - `6`: Dimension of each vector unit in the tensor arrays
# - `float`: The primitive data type of the tensor arrays. Can be `float`/`float32`, `float16`, `bfloat16` or `bit`
db_object.create_table("my_table", {"c1": {"type": "tensorarray,6,float"}}, ConflictType.Ignore)
```

---

### drop_table

```python
db_object.drop_table(table_name, conflict_type = ConflictType.Error)
```

Deletes a table from the database by its name.

#### Parameters

##### table_name: `str`, *Required*

A non-empty string indicating the name of the table to delete.

##### conflict_type: `ConflictType`, *Optional*

- `Error`: Raise an error if the specified table does not exist.
- `Ignore`: Ignore the operation and proceed regardless, if the specified table does not exist.

:::tip NOTE
You may want to import the `infinity.common` package to set `ConflictType`:

```python
from infinity.common import ConflictType
```

:::

:::tip NOTE
If `ConflictType` is not set, it defaults to `Error`.
:::

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
# Delete a table named 'my_table':
# If the specified table does not exist, raise an error. 
db_object.drop_table("my_table")
```

```python
# Delete a table named 'my_table':
# If the specified table does not exist, raise an error (same as above). 
db_object.drop_table("my_table", infinity.common.ConflictType.Error)
```

```python
from infinity.common import ConflictType
# Delete a table named 'my_table':
# If the specified table does not exist, silently ignore the operation and proceed.
db_object.drop_table("my_table", ConflictType.Ignore)
```

---

### add_columns

```python
table_object.add_columns(column_defs)
```

#### Parameters

##### column_defs: `dict[str, dict[str, Any]]`, *Required*

A dictionary defining the columns to add. Each key in the dictionary is a column name (`str`), with a corresponding 'value' dictionary defining the column's data type and default value. See the description of `create_table()`'s `columns_definition` for all available settings.

:::caution NOTE
You must specify a default value each time you add a column.
:::

#### Examples

##### Add an Integer column, a float column, and a varchar/string column at once

```python
table_obj.add_columns({"column_name1": {"type": "integer", "default": 0}, "column_name2": {"type": "float", "default": 0.0}, "column_name3": {"type": "varchar", "default": ""}})
```

##### Add a dense vector column

```python
table_obj.add_columns({"column_name1": {"type": "vector,4,float", "default": [1.0, 1.2, 2.4, 4.6]}})
```

##### Add a multivector column

```python
table_obj.add_columns({"column_name1": {"type": "multivector,4,float", "default": [[1.0, 0.0, 0.0, 0.0], [1.2, 0.0, 0.0, 0.0]]}})
```

##### Add a tensor column

```python
table_obj.add_columns({"column_name1": {"type": "tensor,4,float", "default": [[1.0, 0.0, 0.0, 0.0], [1.2, 0.0, 0.0, 0.0]]}})
```

##### Add a sparse column

```python
from infinity.common import SparseVector
table_obj.add_columns({"column_name1": {"type": "sparse,128,float,int", "default": SparseVector([10, 20, 30], [1.1, 2.2, 3.3])}})
```

Or, you can set the default value in a different format:

```python
table_obj.add_columns({"column_name1": {"type": "sparse,128,float,int", "default": {"10":1.1, "20":2.2, "30": 3.3}}})
```

---

### drop_columns

```python
table_object.drop_columns(column_names)
```

#### Parameters

##### column_names: `list[str]`, *Required*

A list of strings representing the names of the columns to drop.

#### Examples

##### Remove one column from the table

```python
table_object.drop_columns(["column_name"])
```

##### Remove two columns at once

```python
table_object.drop_columns(["column_name1", "column_name2"])
```

---

### get_table

```python
db_object.get_table(table_name)
```

Retrieves a table object by its name.

#### Parameters

##### `table_name`: `str`, *Required*

A non-empty string indicating the name of the table to retrieve.

#### Returns

- Success: An `infinity.remote_infinity.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`:
  - `error_code`: `int` - A non-zero value indicating a specific error condition.
  - `error_msg`: `str` - A message providing additional details about the error.

#### Examples

```python
table_object = db_object.get_table("my_table")
```

---

### show_columns

```python
table_object.show_columns()
```

Show the column definition of the current table.

#### Returns

An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.

:::tip NOTE
This method specifies the projection columns for the current table but does not directly produce displayable data. To display the query results, use `output()` in conjunction with methods like `to_result()`, `to_df()`, `to_pl()`, or `to_arrow()` to materialize the data.
:::

#### Examples

```python
res = table_object.show_columns()
print(res)
```

---

### list_tables

```python
db_object.list_tables()
```

Retrieves a list of all available tables within the current database.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.
- `table_names`: `list[str]` - A list of strings indicating the names of all available tables in the current database.

#### Examples

```python
res = db_object.list_tables()
res.table_names # ['my_table, 'tensor_table', 'sparse_table']
```

---

### show_table

```python
db_object.show_table(table_name)
```

Shows detailed information about a table.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
res = db_object.show_table('my_table')
```

---

### create_index

```python
table_object.create_index(index_name, index_info, conflict_type = ConflictType.Error, comment = None)
```

Creates an index on a specified column.

:::tip NOTE
Call `create_table()` or `get_table()` to get a database object for all index-specific operations.
:::

#### Parameters

##### index_name: `str` *Required*

A non-empty string indicating the name of the index, which must adhere to the following requirements:

- Permitted characters include:
  - English letters (a-z, A-Z)
  - Digits (0-9)
  - "_" (underscore)
- Must begin with an English letter or underscore.
- Maximum 65,535 characters.
- Case-insensitive.

##### index_info: `IndexInfo()`, *Required*

An `IndexInfo` structure contains three fields,`column_name`, `index_type`, and `index_param_list`.

- **column_name**: `str`, *Required*  
  The name of the column to build index on. Must not be empty.  
- **index_type**: `IndexType`, *Required*  
  Index type. You may want to import `infinity.index` to set `IndexType`: `from infinity.index import IndexType`  
  - `Hnsw`: An HNSW index. Works with dense vectors, and multivectors only.
  - `IVF`: An IVF index. Works with dense vectors and multivectors only.
  - `FullText`: A full-text index.  
  - `Secondary`: A secondary index. Works with structured data only.
  - `BMP`: A Block-Max Pruning index. Works with sparse vectors only.
- **index_param_list**: `dict[str, str]`  
  A dictionary specifying the parameter settings for the selected index type. Each key-value pair in the dictionary corresponds to a parameter and its value:
  - Parameter settings for an HNSW index:
    - `"M"`: *Optional* - Defaults to`"16"`.
    - `"ef_construction"`: *Optional* - Defaults to`"50"`.
    - `"metric"` *Required* - The distance metric to use in similarity search.
      - `"ip"`: Inner product.
      - `"l2"`: Euclidean distance.
      - `"cosine"`: Cosine similarity.
    - `"encode"`: *Optional*
      - `"plain"`: (Default) Plain encoding.
      - `"lvq"`: Locally-adaptive vector quantization. Works with float vector element only.  
    - `"build_type"`: *Optional*
      - `"plain"`: (Default) Plain build.
      - `"lsg"`: Local scaling graph.
    - `"lsg_config"`: *Optional* - A dictionary with each key-value pair corresponding to a parameter setting of the local scaling graph:
      - `"sample_raito"`: *Optional* - Defaults to `"0.01"`. The ratio of the number of samples in the dataset (base).
      - `"ls_k"`: *Optional* - Defaults to `"10"`. The number of nearest neighbors of the selected knn mean in local scaling.
      - `"alpha"`: *Optional* - Defaults to `"1.0"`. The smoothing factor in local scaling.
  - Parameter settings for an IVF index:
    - `"metric"` *Required* - The distance metric to use in a similarity search.
      - `"ip"`: Inner product.
      - `"l2"`: Euclidean distance.
      - `"cosine"`: Cosine similarity.
    - `"storage_type"`: *Optional*
      - `"plain"`: (Default) Plain storage.
      - `"scalar_quantization"`: Scalar quantization.
      - `"product_quantization"`: Product quantization.
    - `"plain_storage_data_type"`: *Optional* for plain storage.
      - `"int8"`: default value for `int8` embeddings.
      - `"uint8"`: default value for `uint8` embeddings.
      - `"float32"`: default value for floating-point embeddings.
      - `"float16"`: for floating-point embeddings.
      - `"bfloat16"`: for floating-point embeddings.
    - `"scalar_quantization_bits"`: *Required* for scalar quantization. Must be either `4` or `8`.
    - `"product_quantization_subspace_num"`: *Required* for product quantization. Must be divisor of the embedding dimension.
    - `"product_quantization_subspace_bits"`: *Required* for product quantization. Must be in the range `[4, 16]`.
  - Parameter settings for a full-text index:
    - `"ANALYZER"`: *Optional*
      - `"standard"`: (Default) The standard analyzer, segmented by token, lowercase processing, and provides stemming output. Use `-` to specify the languages' stemmer. `English` is the default stemmer: `"standard-english"` and `"standard"` are the same stemmer setting. Supported language stemmers include: `Danish`, `Dutch`, `English`, `Finnish`, `French`, `German`, `Hungarian`, `Italian`, `Norwegian`, `Porter`, `Portuguese`, `Romanian`, `Russian`, `Spanish`, `Swedish`, and `Turkish`.
      - `"rag"`: Multilingual RAG analyzer imported from [RAGFlow](https://github.com/infiniflow/ragflow/blob/main/rag/nlp/rag_tokenizer.py), supporting `Chinese` and `English`. Use `-fine` to output the fine-grained analyzer results.
      - `"ik"`: Bilingual analyzer imported from [ik-analyzer](https://github.com/infinilabs/analysis-ik), supporting `Chinese` and `English`. Use `-fine` to output the fine-grained analyzer results.
      - `"chinese"`: Simplified Chinese. Use `-fine` to output the fine-grained analyzer results.
      - `"traditional"`: Traditional Chinese.
      - `"japanese"`: Japanese.
      - `"korean"`: Korean.
      - `"ngram"`: [N-gram](https://en.wikipedia.org/wiki/N-gram).
      - `"keyword"`: "noop" analyzer used for columns containing keywords only.
  - Parameter settings for a secondary index:  
    No parameters are required. For now, use an empty list `[]`.
  - Parameter settings for a BMP index:
    - `block_size`: *Optional* - The size of the block in a BMP index. Range: `"1"` ~ `"256"`. Defaults to `"16"`.
    - `"compress_type"`: *Optional*  
      - `"compress"`: (Default) Store the block-max index in sparse format. Works best with small block size situations.
      - `"raw"`: Store the block-max index without compression.

:::tip NOTE
Import the `infinity.index` package to set `IndexInfo`, and `IndexType`.

```python
from infinity.index import IndexInfo, IndexType
```

:::

##### conflict_type: `ConflictType`, *Optional*

- `Error`: Raise an error if an index with the same name exists.
- `Ignore`: Ignore the index creation request and keep the existing table with the same name.

:::tip NOTE
You may want to import the `infinity.common` package to set `ConflictType`:

```python
from infinity.common import ConflictType
```

:::

:::tip NOTE
If `ConflictType` is not set, it defaults to `Error`.
:::

##### comment: `str`, *Optional*

Additional comment for the index to create.

#### Returns

A structure containing these attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

##### Create an HNSW index on a dense vector column

```python {1}
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_hnsw" with a 1024-dimensional float vector column "c1"
table_object = db_object.create_table("test_index_hnsw", {"c1": {"type": "vector,1024,float"}})
# Create an HNSW index named "my_index" on column "c1" with default parameter settings:
# - "M": "16", 
# - "ef_construction": "50",
# - "encode": "plain"
# Only the "metric" parameter (required) is explicitly set to L2 distance. 
table_object.create_index("my_index",IndexInfo("c1", IndexType.Hnsw, {"metric": "l2"}), comment="Index One")
```

```python {1}
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_hnsw" with a 1024-dimensional float vector column "c1"
table_object = db_object.create_table("test_index_hnsw", {"c1": {"type": "vector,1024,float"}})
# Create an HNSW index named "my_index" on column "c1"
# Settings for "M", "ef_construction", and "metric" are the same as above, except:
# "encoding" is set to "lvq" 
table_object.create_index(
    "my_index",
        IndexInfo(
            "c1",
            IndexType.Hnsw,
            {
              "M": "16",
              "ef_construction": "50",
              "metric": "l2",
              "encode": "lvq"
            }
        )
)
```

##### Create an HNSW index on a multi-vector column

```python
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_hnsw" with a 4-dimensional float multivector column "c1"
table_object = db_object.create_table("test_index_hnsw", {"c1": {"type": "multivector,4,float"}})
# Create an HNSW index named "my_index" on column "c1" with default parameter settings:
# - "M": "16", 
# - "ef_construction": "50",
# - "encode": "plain"
# Only the "metric" parameter (required) is explicitly set to L2 distance. 
table_object.create_index("my_index", IndexInfo("c1", IndexType.Hnsw, {"metric": "l2"}))
```

##### Create a full-text index

```python
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_fulltext" with a varchar column "body"
table_object = db_object.create_table("test_index_fulltext", {"body": {"type": "varchar"}})
# Create a full-text index named "my_index" on column "body" with default parameter settings:
# - "ANALYZER": "standard"
table_object.create_index(
    "my_index",
        IndexInfo(
            "body", 
            IndexType.FullText, 
        )
)
```

```python
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_fulltext" with a varchar column "body"
table_object = db_object.create_table("test_index_fulltext", {"body": {"type": "varchar"}})
# Create a full-text index named "my_index" on column "body"
# Setting "ANALYZER" to "standard" (same as the above)
table_object.create_index(
    "my_index",
        IndexInfo(
            "body", 
            IndexType.FullText, 
            {
                "ANALYZER": "standard"
            }
        )
)
```

##### Create a secondary index

```python
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_secondary" with a varchar column "body"
table_object = db_object.create_table("test_index_secondary", {"c1": {"type": "varchar"}})
# Create a secondary index named "my_index" on column "c1"
table_object.create_index(
    "my_index",
        IndexInfo(
            "c1", 
            IndexType.Secondary 
        )
)
```

##### Create a BMP index

```python
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_bmp" with a sparse vector column "c1"
table_object = db_object.create_table("test_index_bmp", {"c1": {"type": "sparse,30000,float,int16"}})
# Create a BMP index named "my_index" on column "c1" with default parameter settings:
# - "block_size": "16"
# - "compress_type": "compress"
table_object.create_index(
    "my_index",
        IndexInfo(
            "c1",
            IndexType.BMP
        )
)
```

```python
from infinity.index import IndexInfo, IndexType
# Create a table named "test_index_bmp" with a sparse vector column "c1"
table_object = db_object.create_table("test_index_bmp", {"c1": {"type": "sparse,30000,float,int16"}})
# Create a BMP index named "my_index" on column "c1"
# Settings for "block_size" and "compress_type" are the same as above
table_object.create_index(
    "my_index",
        IndexInfo(
            "c1",
            IndexType.BMP,
            {
              "block_size": "16",
              "compress_type": "compress"
            }
        )
)
```

---

### optimize_index

```python
table_obj.optimize(index_name, opt_params: dict[str, str])
```

Optimize an index by its name.

#### Parameters

##### index_name: `str`, *Required*

A non-empty string indicating the name of the index to optimize.

##### opt_params: `dict[str, str]`, *Required*

A dictionary specifying the optimization parameters for the selected index type. Each key-value pair in the dictionary corresponds to a parameter and its value:

- Parameters settings for an HNSW index:
  - `"compress_to_lvq"`: *Optional* - Defaults to `"false"`. Compress existing plain HNSW index to LVQ index.
  - `"lvq_avg"`: *Optional* - Defaults to `"false"`. Calculate the average of the LVQ index.
- Parameters settings for an BMP index:
  - `"topk"`: *Optional* - Optimize bmp index for topk search.
  - `"bp_reorder"`: *Optional* - Defaults to `"false"`. Reorder internal sequence of the bmp index by heuristic rules(binary partition).

---

---

### drop_index

```python
table_object.drop_index(index_name, conflict_type = ConflictType.Error)
```

Deletes an index by its name.

#### Parameters

##### index_name: `str`, *Required*

A non-empty string indicating the name of the index to delete.

##### conflict_type: `ConflictType`, *Optional*

- `Error`: Raise an error if an index with the specified name does not exist.
- `Ignore`: Ignore the index creation request if the index does not exist.

:::tip NOTE
You may want to import the `infinity.common` package to set `ConflictType`:

```python
from infinity.common import ConflictType
```
:::

:::tip NOTE
If `ConflictType` is not set, it defaults to `Error`.
:::

#### Returns

A structure containing these attributes:

- `error_code`: `int`  
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.  

#### Examples

```python
table_object.drop_index("my_index")
```

---

### list_indexes

```python
table_object.list_indexes()
```

Retrieves a list of all available indexes built on the current table.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.
- `table_names`: `list[str]` - A list of strings indicating the names of all available indexes.

#### Examples

```python
res = table_object.list_indexes()
res.index_names # ['my_index', 'tensor_index', 'sparse_index']
```

---

### show_index

```python
table_object.show_index(index_name)
```

Shows detailed information about an index.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
res = table_object.show_index('my_index')
```

---

### show_current_node

```python
infinity.show_current_node()
```

Shows the role of the currently connected node.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.
- `role`: `str`  
  - `"admin"`
  - `"standalone"`
  - `"leader"`
  - `"follower"`
  - `"learner"`.

#### Examples

```python
res = infinity_object.show_current_row()
```

---

## DATA MANAGEMENT

---

### insert

```python
table_object.insert(data)
```

Inserts rows of data into the current table.

#### Parameters

##### data: `dict[str, Any]`, *Required*

Data to insert. Infinity supports inserting multiple rows to a table at one time in the form of `dict[str, Any]` (one row) or `list[dict[str, Any]]` (multiple rows), with each key-value pair corresponding to a column name and a table cell value.

:::tip NOTE
- When inserting incomplete rows of data, ensure that all un-inserted columns have default values when calling `create_table()`. Otherwise, an error will occur.
- You are not allowed to insert both complete and incomplete rows of data in one request.
For information about setting default column values, see `create_table()`.
:::

:::tip NOTE
Batch row limit: 8,192. You are allowed to insert a maximum of 8,192 rows at once.
:::

#### Returns

A structure containing the following attributes:

- `error_code`: `int`  
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

##### Insert primitives

```python
# Create a table with four primitive columns:
table_instance = db_instance.create_table("primitive_table", {
    "c1": {"type": "int8", "default": 0},
    "c2": {"type": "int16", "default": 0},
    "c3": {"type": "int", "default": 0},
    "c4": {"type": "int32", "default": 0},   # Same as int
    "c5": {"type": "integer", "default": 0}, # Same as int
    "c6": {"type": "int64", "default": 0},
    "c7": {"type": "varchar"},
    "c8": {"type": "float", "default": 1.0},
    "c9": {"type": "float32", "default": 1.0}, # Same as float
    "c10": {"type": "double", "default": 1.0},
    "c11": {"type": "float64", "default": 1.0}, # Same as double
    "c12": {"type": "bool", "default": False},
})

# Insert an incomplete row, with remaining cells defaulting to their column defaults:
table_instance.insert({"c1": 1, "c7": "Tom", "c12": True})
```

##### Insert dense vectors

```python
# Create a table with an integer column and a 3-d vector column:
table_object = db_object.create_table("vector_table", {"c1": {"type": "integer", "default": 2024}, "vector_column": {"type": "vector,3,float"}})

# Insert one incomplete row into the table:
# Note that the 'c1' cell defaults to 0. 
table_object.insert({"vector_column": [1.1, 2.2, 3.3]})

# Insert two incomplete rows into the table:
# Note that the 'c1' cells default to 0. 
table_object.insert([{"vector_column": [1.1, 2.2, 3.3]}, {"vector_column": [4.4, 5.5, 6.6]}])
```

##### Insert sparse vectors

```python
from infinity.common import SparseVector
# Create a table with an integer column and a 100-d sparse vector column:
table_object = db_object.create_table("sparse_vector_table", {"c1": {"type": "integer"}, "sparse_column": {"type": "sparse,100,float,int"}})

# Insert one row into the table:
# `indices` specifies the corresponding indices to the values in `values`.
# Note that the second row sets "c1" as 2024 by default. 
table_object.insert([{"c1": 2022, "sparse_column": SparseVector([10, 20, 30], [1.1, 2.2, 3.3])}, {"sparse_column": SparseVector([70, 80, 90], [7.7, 8.8, 9.9])}])
```

##### Insert tensors

```python
# Create a table with a tensor column: 
table_object = db_object.create_table("tensor_table", {"c1": {"type": "integer", "default": 2024}, "tensor_column": {"type": "tensor,4,float"}})

# Insert one row into the table:
table_instance.insert([{"tensor_column": [[1.0, 0.0, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]]}])
```

##### Insert tensor arrays

```python
# Creat a table with only one tensor array column:
table_object = db_object.create_table("tensor_array_table", {"tensor_array_column": {"type": "tensorarray,2,float"}})
table_object.insert([{"tensor_array_column": [[[1.0, 2.0], [3.0, 4.0]], [[5.0, 6.0]]]}])
```

##### Insert arrays

```python
from infinity.common import Array
# Creat a table with only one varchar array array column:
table_object = db_object.create_table("varchar_array_array_table",
                                      {"varchar_array_array_column": {"type": "array,array,varchar"}})
# Insert one row into the table:
table_object.insert([{"varchar_array_array_column": Array(Array("hello", "world"), Array("!"), Array())}])
```

---

### import_data

```python
table_object.import_data(filepath, import_options)
```

Imports data from a specified file into the current table.

#### Parameters

##### file_path: `str`, *Required*

Absolute path to the file for export.

##### import_options: `dict[str, bool | str]`

Example: `{"header":True, "delimiter": "\t", file_type}`

- **header**: `bool`  
  Whether to display table header or not. Works with **.csv** files only:
  - `True`: Display table header.
  - `False`: (Default) Do not display table header.

- **delimiter**: `str`, *Optional*, Defaults to ","  
  Delimiter to separate columns. Works with **.csv** files only.

- **file_type**: `str`, *Required*  
  The type of the imported file. Supported file types include:
  - `csv`
  - `json`
  - `jsonl`
  - `parquet`

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

##### Import a csv file

```python
table_object.import_data(os.getcwd() + "/your_file.csv", {"header": False, "file_type": "csv", "delimiter": "\t"})
```

##### Import a jsonl file

```python
table_object.import_data(os.getcwd() + "/your_file.jsonl", {"file_type": "jsonl"})
```

##### Import a parquet file

```python
table_object.import_data(os.getcwd() + "/your_file.parquet", {"file_type": "parquet"})
```

---

### export_data

```python
table_object.export_data(filepath, export_options, columns = None)
```

Exports data in the current table to a specified file.

#### Parameters

##### file_path: `str`, *Required*

Absolute path to the file for export.
  
##### export_options: `dict[str, Any]`, *Required*

Example: `{"header": False, "delimiter": "\t", "file_type": "jsonl", "offset": 2, "limit": 5}`

- **header**: `bool`, *Optional*  
  Whether to display table header or not. Works with **.csv** files only:
  - `True`: Display table header.
  - `False`: (Default) Do not display table header.

- **delimiter**: `str`, *Optional*, Defaults to ","  
  Delimiter to separate columns. Works with **.csv** files only.

- **file_type**: `str`, *Required*  
  The type of the exported file. Supported file types include:
  - `csv`
  - `jsonl`
  - `parquet`
  
- **offset**: `int`, *Optional*  
  Index specifying the starting row for export. Usually used in conjunction with `limit`. If not specified, the file export starts from the first row. 

- **limit**: `int`, *Optional*  
  The maximum number of rows to export. Usually used in conjunction with `offset`. If the table's row count exceeds `offset` + `limit`, the excess rows are excluded from the export.

- **row_limit**: `int`, *Optional*  
  Used when you have a large table and need to break the output file into multiple parts. This argument sets the row limit for each part. If you specify **test_export_file.csv** as the file name, the exported files will be named **test_export_file.csv**, **test_export_file.csv.part1**, **test_export_file.csv.part2**, and so on.

##### columns: `[str]`, *Optional*

Columns to export to the output file, for example, `["num", "name", "score"]`. If not specified, the entire table is exported.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

##### Export your table to a csv file

```python
table_object.export_data(os.getcwd() + "/export_data.csv", {"header": True, "file_type": "csv", "delimiter": ",", "offset": 2, "limit": 7, "row_limit": 3}, ["num", "name", "score"])
```

##### Export your table to a jsonl file

```python
table_object.export_data(os.getcwd() + "/export_data.jsonl", {"file_type": "jsonl", "offset": 1, "limit": 8, "row_limit": 2}, ["num", "name", "score"])
```

##### Export your table to a parquet file

```python
table_object.export_data(os.getcwd() + "/export_data.parquet", {"file_type": "parquet")
```

---

### delete

```python
table_object.delete(cond = None)
```

Deletes rows from the table based on the specified condition.

#### Parameters

##### cond: `str`, *Optional*

A string that defines the condition for selecting rows to delete. The parameter can be an expression, a function, or any other form of conditional logic that evaluates to `True` for the rows that should be deleted. If `cond` is not specified or set to `None`, the method will delete all rows in the table.

:::tip NOTE

- The `cond` parameter currently supports 'and' and 'or' logical expressions only.
- `cond` must not be an empty string.
:::

#### Returns

A structure containing the following attributes:

- `error_code`: `int`  
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.
- `deleted_rows`: `int`
  The number of deleted rows.

#### Examples

##### Remove all rows in the table

```python
# Clear all data in the current table
table_object.delete()
```

```python
# Clear all data in the current table
table_object.delete(None)

```

##### Conditional row deletion

```python
# Create a table named "my_table" with two columns:
# - Integer column "c1"
# - Vector column "vec"
table_object = db_instance.create_table("my_table", {"c1": {"type": "integer"}, "vec": {"type": "vector,4,float"},})
# Insert two rows of data into the "my_table"
table_object.insert([{"c1": 90, "vec": [1.0, 1.2, 0.8, 0.9],}, {"c1": 80, "vec": [4.0, 4.2, 4.3, 4.5],},])
# Delete rows where "c1" equals 1
table_object.delete("c1 = 90")
```

```python
# Create a table named "my_table" with one integer column "c1"
table_object = db_instance.create_table("my_table", {"c1": {"type": "integer"}})
# Insert three rows of data into the "my_table"
table_object.insert([{"c1": 90}, {"c1": 80}, {"c1": 95}])
# Delete rows where "c1" is between 70 and 90 (inclusive)
table_object.delete("c1 >= 70 and c1 <= 90")
```

---

### update

```python
table_object.update(cond, data)
```

Searches for rows that match the specified condition and updates them accordingly.

#### Parameters

##### cond: `str`, *Required*

A string that defines the condition for selecting rows to update. It represents a logical expression, a function, or any other form of conditional logic that evaluates to `True` for the rows that should be updated. If `cond` is not specified or set to `Null`, the method will update all rows in the table.

##### data: `dict[str, Any]`, *Required*

A non-empty dictionary where each key indicates a column name and each value indicates the new value for the corresponding cell.

#### Returns

A structure containing the following attributes:

- `error_code`: `int`
  - `0`: The operation succeeds.
  - A non-zero value indicates a specific error condition.
- `error_msg`: `str`  
  When `error_code` is non-zero, `error_msg` provides additional details about the error.

#### Examples

```python
# Update rows where column "c1" equals 1, setting "c2" to 90 and "c3" to 900
table_object.update("c1 = 1", {"c2": 90, "c3": 900})
```

```python
# Update rows where column "c1" is greater than 2, setting "c2" to 100 and "c3" to 1,000
table_object.update("c1 > 2", {"c2": 100, "c3": 1000})
```

---

## SEARCH

---

### output

```python
table_object.output(columns)
```

This method allows you to customize the output of your query by selecting specific columns, applying aggregation functions, or performing arithmetic operations.

#### Parameters

##### columns: `list[str]`, *Required*

A non-empty list of strings specifying the columns to include in the output. Each string in the list can represent:

- A user-defined column name: The name of the column to include in the output, e.g., `"body"`.
- All user-defined columns: Use a wildcard `"*"` to select all columns.
- A special system column: system-generated columns include:
  - `_row_id`:  An automatically generated, unique identifier for each row in the table. It serves as a unique key for each row but does not necessarily correspond to the actual row number. When the data in a row is updated, the `_row_id` for that row is also changed to reflect the update.
  - `_score`: A BM25 score used in full-text search.
  - `_similarity`: Used by IP and cosine metrics in dense or sparse vector search.
  - `_distance`: Used by L2 metric in dense vector search.
- An aggregation function: Apply an aggregation operation on specified columns. Supported aggregation functions include:
  - `count`
  - `min`
  - `max`
  - `sum`
  - `avg`
- An arithmetic function: Apply an arithmetic operation on specified columns (e.g., `c1+5`).
- An unnest function: Unnest an array column to multiple rows (e.g., `unnest(c1)`).

:::tip NOTE
The list must contain at least one element. Empty lists are not allowed.
:::
  
#### Returns

An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.

:::tip NOTE
This method specifies the projection columns for the current table but does not directly produce displayable data. To display the query results, use `output()` in conjunction with methods like `to_result()`, `to_df()`, `to_pl()`, or `to_arrow()` to materialize the data.
:::

#### Examples

##### Select columns to display

```python
# Select all columns
table_object.output(["*"]).to_pl()
```

```python
# Select columns "num" and "body"
table_object.output(["num", "body"]).to_df()
```

```python
# Select a system-generated column "_row_id"
table_object.output(["_row_id"]).to_pl()
```

##### Select unnest columns to 

```python
# Select column "c1" and unnest its cells
table_object.output(["unnest(c1)"]).to_pl()
```

##### Perform aggregation or arithmetic operations on selected columns

```python
# Specify that the output should display the average value of all cells in column "c2"
table_object.output(["avg(c2)"]).to_pl()
```

```python
# Select column "c1" and request all cells in this column to be displayed with their original values increased by 5
table_object.output(["c1+5"]).to_pl()
```

```python
# Specify that the output should display the result of an arithmetic operation combining two aggregation functions
table_object.output(["min(c1) + max(c2)"]).to_pl()
```

```python
# Specify that the output should display the row number of the current table
table_object.output(["count(*)"]).to_pl()
```

```python
# Select column "num" and request all cells in this column to be displayed with their original values divided by 10
table_object.output(["num / 10"]).to_pl()
```

```python
# Select column "num" and display all its cells as absolute values
table_object.output(["abs(num)"]).to_pl()
```

```python
# Specify that the output should display the result of three multiplied by five
# Note that no columns are involved in this example!
# Either of the following works: 
table_object.output(["3 * 5"]).to_pl()
```

---

### filter

```python
table_object.filter(cond)
```

Creates a filtering condition expression for the current table.

:::tip NOTE
This method creates a filtering condition for your query. To display the results, you must chain it with `output(columns)`, which specifies the columns to output, and a method such as `to_pl()`, `to_df()`, or `to_arrow()` to format the query results.
:::

#### Parameters

##### cond: `str`, *Required*

A non-empty string representing the filter condition. It comprises one or multiple expressions combined by 'and', 'or' or 'not' logical operators, where each expression uses comparison operators to set criteria for keeping or removing rows.

##### filter expressions in cond

* `<`, `<=`, `>`, `>=`, `=`, `==`, `!=` expression
* `in` and `not in` expression
* `filter_fulltext` expression
  - Similar to `match_text()`.
    Usage: 'filter_fulltext(fields, matching_text)' or 'filter_fulltext(fields, matching_text, extra_options)'
  - 'extra_options' is in the format of 'K1=V1;K2=V2;...;KN=VN',
    where each 'K' represents a parameter name and 'V' represents its value
  - Available parameters in 'extra_options':
    - **'minimum_should_match'**: specifies how many clauses in the 'matching_text' should be satisfied at least.
      It can be in the following format:
      - Positive integer `N`: at least `N` clauses should be satisfied.
      - Negative integer `-N`: at least (total clause count - `N`) clauses should be satisfied.
      - Positive Percentage `N%`: at least `⌊total clause count * N%⌋` clauses should be satisfied.
      - Negative Percentage `-N%`: at least `total clause count - ⌊total clause count * N%⌋` clauses should be satisfied.
      - Combination `K<V`: `K` is positive integer, `V` is in one of four styles described above.
        This means that when `total clause count > K`, the requirement `V` applies, otherwise all the clauses should be satisfied
      - Multiple combinations `K1<V1 K2<V2 ... KN<VN`: several `K<V` strings seperated by spaces, with `K` in the ascending order.
        If `K1 >= total clause count`, all the clauses should be satisfied.
        Otherwise, we find the biggest `V` which is less than the total clause count and apply the correspondent `V`.
    - **'default_field'**
      - If `"fields"` is an empty string, this parameter specifies the default field to search on.

##### Returns

An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.

:::tip NOTE
This method specifies a filtering condition for the rows in the current table but does not directly produce displayable data. To display the query results, use `filter()` in conjunction with methods like `to_result()`, `to_df()`, `to_pl()`, or `to_arrow()` to materialize the data.
:::

#### Examples

```python
table_object.output(["c1", "c2"]).filter("(-7 < c1 or 9 >= c1) and (c2 = 3)").to_pl()
```

```python
table_object.output(["*"]).filter("c2 = 3").to_pl()
```

```python
table_object.output(["*"]).filter("c1 not in (1, 2, 3) and (c2 + 1) in (1, 2, 3)").to_df()
```

```python
table_object.output(["*"]).filter("filter_fulltext('doc', 'first second', 'minimum_should_match=99%') and not num = 2").to_pl()
```

---

### sort

```python
table_object.sort(sort_expression_list)
```

Creates a sort expression using `sort_expression_list`.

#### Parameters

##### sort_expression_list: `list`, *Required*

An expression list defining how to sort the results.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

```python
# Output results sorted by the `c2` expression in ascending order and the `c1` expression in descending order
table_obj.output(["c1", "c2"]).sort([["c2", SortType.Asc], ["c1", SortType.Desc]]).to_df()
```

---

### group_by

```python
table_object.group_by(group_by_columns)
```

Creates a group-by expression using `group_by_columns`.

#### Parameters

##### group_by_columns: `list[str] | str`, *Required*

A list of strings specifying the expression to group by. Each string in the list represents a column name or an expression.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

```python
table_obj.output(["c1", "sum(c2)"]).group_by(["c1"]).to_df()
table_obj.output(["c1", "avg(c1)", "count(c2)", "min(c3)", "max(c4)"]).group_by(["c1", "c1+c2"]).to_df()
```

---

### having

```python
table_object.having(expr)
```

Creates a filtering condition expression for the group-by result.

#### Parameters

##### having_expr: `str`, *Required*

A string specifying the having expression.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

```python
table_obj.output(["c1", "sum(c2)"]).group_by(["c1"]).having("sum(c2) > 10").to_df()
```

---

### limit

```python
table_object.limit(limit_num)
```

Creates an expression to limit the number of the output rows to a maximum of `limit_num`.

#### Parameters

##### limit_num: `int`, *Required*

An integer specifying the maximum number of output rows.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

```python
# Limit the output row count to a maximum of two
table_instance.output(["num", "vec"]).limit(2).to_pl()
```

---

### offset

```python
table_object.limit(limit_num).offset(offset_value)
```

Creates a limit expression with an offset value, setting the output to start from `offset_value` and limiting the row count to a maximum of `limit_num`. This method must be used in conjunction with `limit()`.

#### Parameters

##### offset_value: `int`, *Required*

An integer specifying the offset position of the limit expression.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

```python
# Limit the output row count not more than 2, start from position 1
table_instance.output(["num", "vec"]).offset(1).limit(2).to_pl()
```

### option

```python
table_object.option(option_dict)
```

Indicates some search options.

#### Parameters

##### option_dict: `dict`, *Required*

A dictionary specifying the following search options:

- **"total_hits_count"**: `bool`, *Optional*
  - Must combine with limit expression. If `"total_hits_count"` is `True`, the query will output an extra result including total hits row count of the query.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

```python
# Limit the output row count not more than 2, start from position 1, output an extra result to indicate total hits row count
table_instance.output(["num", "vec"]).limit(2).offset(1).option({"total_hits_count": True}).to_pl()
```

---

### match_dense

```python
table_object.match_dense(vector_column_name, embedding_data, embedding_data_type, distance_type, topn, knn_params = None)
```

Creates a dense vector search expression to identify the closest top n rows to the given dense vector. Suitable for working with dense vectors (dense embeddings) or multi-vectors (multiple dense embeddings in one row).

:::tip NOTE
To display your query results, you must chain this method with `output(columns)`, which specifies the columns to output, and a method such as `to_pl()`, `to_df()`, or `to_arrow()` to format the query results.
:::

#### Parameters

##### vector_column_name: `str`, *Required*

A non-empty string indicating the name of the vector multi-vector column to search on.

##### embedding_data: `list/np.ndarray`, *Required*

The query vector data to compare against. This should be provided as a list or a one-dimensional NumPy array of numerical values.

##### embedding_data_type: `str`, *Required*

Specifies the data type of the embedding vector. Commonly used types (values) include:

- `"float"`
- `"uint8"`.

##### distance_type: `str`, *Required*

The distance metric to use in similarity search.

- `"ip"`: Inner product.
- `"l2"`: Euclidean distance.
- `"cosine"`: Cosine similarity.

##### topn: `int`, *Required*

An integer indicating the number of nearest neighbours to return.

##### knn_params: `dict[str, str]`, *Optional*

A dictionary representing additional KNN or ANN search parameters.

- `"ef"`: `str`, Recommended value: one to ten times the value of `topn`.  
  - For example, if you set `topn` to `10`, you can set `"ef"` to `"50"`.
  - If you set `"ef"` too high, search performance may worsen.  
  - If you do not set `"ef"` or set it to a value lower than `topn`, the search uses the `topn` value as the value for `"ef"`.
- `"threshold"`: `str`, *Optional* A threshold value for the search.
  - For example, if you use the `"cosine"` distance metric and set `"threshold"` to `"0.5"`, the search will return only those rows with a cosine similarity greater than `0.5`.
- `"nprobe"`: `str`, *Optional* The number of cells to search for the IVF index. The default value is `"1"`.
- `"index_name"` : `str`, *Optional* The name of index on which you would like the database to perform query on.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

##### Perform a brute-force vector search

```python
# Find the 100 nearest neighbors using Euclidean distance
# If no vector index is created on the column being queried, then the vector search defaults to a brute-force search.
# In such case, set `knn_params` to `None` or leave it blank.
table_object.output(["*"]).match_dense("vec", [0.1,0.2,0.3], "float", "l2", 100).to_pl()
```

:::caution NOTE
`knn_params` settings will not take effect here because no index has been created.
:::

##### Perform a vector search in HNSW

1. Ensure that you have successfully built an HNSW index. If uncertain, you can rebuild the index, setting `ConflictType` to `Ignore`.
2. Set the `ef` value as follows:

```python
from infinity.index import IndexInfo, IndexType
table_object.create_index("my_index", IndexInfo("vec", IndexType.Hnsw, {"ef_construction": "50"}))
# Find the 2 nearest neighbors using cosine distance
# If an HNSW index is successfully built on the column being queried, then the vector search uses this index,
# regardless of whether `knn_params` is set.
# If you leave `knn_params` blank, the search uses the `topn` value as the value for `"ef"`.
table_object.output(["*"]).match_dense("vec", [1, 2, 3], "uint8", "cosine", 2).to_pl()
```

```python
from infinity.index import IndexInfo, IndexType
table_object.create_index("my_index", IndexInfo("vec", IndexType.Hnsw, {"ef_construction": "50"}))
# Find the 2 nearest neighbors using inner product distance
# If an HNSW index is successfully built on the column being queried, then the vector search uses this index,
# regardless of whether `knn_params` is set.
table_object.output(["*"]).match_dense("vec", [0.1,0.2,0.3], "float", "ip", 2, {"ef": "100"}).to_pl()
```

:::tip NOTE
If the HNSW index is not created successfully, the search will fall back to a brute-force search.
:::

---

### match_sparse

```python
table_object.match_sparse(vector_column_name, sparse_data, distance_type, topn, opt_params)
```

Creates a sparse vector search expression to identify the top n closest rows to the given sparse vector. Suitable for working with sparse vectors (sparse embeddings).

:::tip NOTE
To display your query results, you must chain this method with `output(columns)`, which specifies the columns to output, and a method such as `to_pl()`, `to_df()`, or `to_arrow()` to format the query results.
:::

#### Parameters

##### vector_column_name: `str`, *Required*

A non-empty string indicating the name of the column to query on.

##### sparse_data: `SparseVector(list[int], list[int] | list[float])`, *Required*

The query sparse vector data to compare against. The `sparse_data` parameter should be provided as a SparseVector object, which has two members:

- `indices`: A list of the indices, each corresponding to a non-zero value in the sparse vector.
- `values`: A list of the corresponding values for each index in the `indices` list.

:::tip NOTE
If you have a dictionary of indices and values, you can create a SparseVector object using the `SparseVector` class. For example:

```python
from infinity.common import SparseVector
dic_sparse_vector = {"indices": [0, 10, 20], "values": [0.1, 0.2, 0.3]}
sparse_vector = SparseVector(**dic_sparse_vector)
```

:::

##### distance_type: `str`, *Required*

 A non-empty string indicating the distance type for the search. Currently, only `"ip"` (inner product) is supported.

##### topn: `int`, *Required*

An integer indicating the number of nearest neighbours to return.

##### opt_params: `dict[str, str]`, *Optional*

A dictionary representing additional parameters for the sparse vector search. Following are parameters for the BMP index:

- `"alpha"`: `str`  
  `"0.0"` ~ `"1.0"` (default: `"1.0"`) - A "Termination Conditions" parameter. The smaller the value, the more aggressive the pruning.
- `"beta"`: `str`  
  `"0.0"` ~ `"1.0"` (default: `"1.0"`) - A "Query Term Pruning" parameter. The smaller the value, the more aggressive the pruning.
- `"index_name"` : `str`, *Optional* The name of index on which you would like the database to perform query on.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

##### Perform a brute-force sparse vector search

```python
# As demonstrated in the following example:
# The sparse vector search is performed on column "sparse_column" to find the 100 nearest neighbors using inner product
# SparseVector([0, 10, 20], [0.1, 0.2, 0.3]) represents the sparse vector to compare against:
# - 0: the index of 0.1
# - 10: the index of 0.2
# - 20: the index of 0.3
# If no sparse vector index is created on the column being queried, then the search defaults to a brute-force search.
# In such case, set `opt_params` to `None` or leave it blank.
from infinity.common import SparseVector
table_object.output(["*"]).match_sparse('sparse_column', SparseVector([0, 10, 20], [0.1, 0.2, 0.3]), 'ip', 100).to_df()
```

:::caution NOTE
`opt_params` settings will not take effect here because no index has been created.
:::

##### Perform a sparse vector search in BMP

```python
from infinity.index import IndexInfo, IndexType
table_object.create_index("my_index", [IndexInfo("sparse_column", IndexType.BMP)])
# Find the 100 nearest neighbors using inner product
# If a BMP index is successfully built on the column being queried, then the sparse vector search uses this index,
# regardless of whether `opt_params` is set.
# If you leave `opt_params` blank, the search takes the default settings for `"alpha"` and `"beta"`.
from infinity.common import SparseVector
table_object.output(["*"]).match_sparse('sparse_column', SparseVector([0, 10, 20], [0.1, 0.2, 0.3]), 'ip', 100, {"alpha": "1.0", "beta": "1.0"}).to_df()
```

```python
from infinity.index import IndexInfo, IndexType
table_object.create_index("my_index", IndexInfo("sparse_column", IndexType.BMP))
# Find the 100 nearest neighbors using inner product
# If a BMP index is successfully built on the column being queried, then the sparse vector search uses this index,
# regardless of whether `opt_params` is set.
# You can set the values of `"alpha"` or `"beta"` in `opt_params`, which overrides the default settings.
from infinity.common import SparseVector
table_object.output(["*"]).match_sparse('sparse_column', SparseVector([0, 10, 20], [8, 10, 66]), 'ip', 100, {"alpha": "1.0", "beta": "1.0"}).to_df()
```

---

### match_text

```python
table_object.match_text(fields, matching_text, topn, extra_options)
```

Creates a full-text search expression on the specified field(s)/column(s) to identify the most relevant rows.

:::tip NOTE
To display your query results, you must chain this method with `output(columns)`, which specifies the columns to output, and a method such as `to_pl()`, `to_df()`, or `to_arrow()` to format the query results.
:::

#### Parameters

##### fields: `str`, *Required*

A non-empty, comma-separated string of column names on which the full-text search will be performed.

:::danger NOTE
Ensure that a full-text index has been successfully built on each column involved before executing a full-text search; otherwise, an error will occur.
:::

:::tip NOTE
To display your query results, you must chain this method with `output(columns)`, which specifies the columns to output, and a method such as `to_pl()`, `to_df()`, or `to_arrow()` to format the query results.
:::

##### matching_text: `str`, *Required*

A non-empty text string to search for. You can use various search options within the matching text, including:

- Single terms: `"blooms"`
- OR multiple terms: `"Bloom OR filter"` or just `"Bloom filter"`
- Phrase search: `'"Bloom filter"'`
- AND multiple terms: `"space AND efficient"`
- Escaping reserved characters: `"space\:efficient"`
- Sloppy phrase search: `'"harmful chemical"~10'`
- Field-specific search: `"title:(quick OR brown) AND body:foobar"`

##### topn: `int`, *Required*

Specifies the number of the most relevant rows to retrieve, e.g., assign `10` to obtain the ten most relevant rows.

##### extra_options: `dict`, *Optional*

An optional dictionary specifying the following search options:

- **"default_field"**: `str`, *Optional*
  - If `"fields"` is an empty string, this parameter specifies the default field to search on.
- **"operator"**: `str`, *Optional*
  - If not specified, the search follows Infinity's full-text search syntax, meaning that logical and arithmetic operators, quotation marks and escape characters will function as full-text search operators, such as:
    - AND operator: `AND`
    - OR operator: `OR`
    - NOT operator: `NOT`
    - PAREN operator: `(`, `)`, need to appear in pairs, and can be nested.
    - COLON operator: `:`: Used to specify field-specific search, e.g., `body:foobar` searches for `foobar` in the `body` field.
    - CARAT operator: `^`: Used to boost the importance of a term, e.g., `quick^2 brown` boosts the importance of `quick` by a factor of 2, making it twice as important as `brown`.
    - TILDE operator: `~`: Used for sloppy phrase search, e.g., `"harmful chemical"~10` searches for the phrase `"harmful chemical"` within a tolerable distance of 10 words.
    - SINGLE_QUOTED_STRING: Used to search for a phrase, e.g., `'Bloom filter'`.
    - DOUBLE_QUOTED_STRING: Used to search for a phrase, e.g., `"Bloom filter"`.
    - Escape characters: Used to escape reserved characters, e.g., `space\:efficient`. Starting with a backslash `\` will escape the following characters:   
      `' '`, `'('`, `')'`, `'^'`, `'"'`, `'\''`, `'~'`, `'*'`, `'?'`, `':'`, `'\\'`
  - If specified, Infinity's full-text search syntax will not take effect, and the specified operator will be interpolated into `matching_text`.  
    Useful for searching text including code numbers like `"A01-233:BC"`.
    - `{"operator": "or"}`: Interpolates the `OR` operator between words in `matching_text` to create a new search text.  
      For example, reinterprets `"A01-233:BC"` as `'"A01" OR "-233" OR "BC"'`.
    - `{"operator": "and"}`: Interpolates the `AND` operator between words in `matching_text` to create a new search text.  
      For example, reinterprets `"A01-233:BC"` as `'"A01" AND "-233" AND "BC"'`.
  - **`"index_name"`** : `str`, *Optional* The names of indexes on which you would like the database to perform query on.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

```python

questions = [
    r"blooms",  # single term
    r"Bloom filter",  # OR multiple terms
    r'"Bloom filter"',  # phrase: adjacent multiple terms
    r"space efficient",  # OR multiple terms
    r"space\-efficient",  # Escape reserved character '-', equivalent to: `space efficient`
    r'"space\-efficient"',  # phrase and escape reserved character, equivalent to: `"space efficient"`
    r'"harmful chemical"~10',  # sloppy phrase, refers to https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-match-query-phrase.html
    r'title:(quick OR brown) AND body:foobar', # search `(quick OR brown)` in the `title` field. keep fields empty.
]
for question in questions:
    table_object.output(["*"]).match_text('body', question, 2).to_df()
    table_object.output(["*"]).match_text('', question, 2, {'default_field': 'body'}).to_df()
```

---

### highlight

```python
table_object.highlight(columns)
```

This method allows you to highlight the words of the output of the query, when the word text are matched.

#### Parameters

##### columns: `list[str]`, *Required*

A list of strings specifying the columns to include words match in match_text clause. Each string in the list can represent:

- A user-defined column name: The name of the column to include in the output, e.g., `"body"`.

:::tip NOTE
The columns must also be the `output` clause output and also be the match_text clause column.
:::

#### Returns

An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.

:::tip NOTE
This method specifies the projection columns for the current table but does not directly produce displayable data. To display the query results, use `output()` in conjunction with methods like `to_result()`, `to_df()`, `to_pl()`, or `to_arrow()` to materialize the data.
:::

#### Examples

##### Highlight the matched column to display

```python
table_obj.output(["doctitle", "docdate", "body"]).highlight(["body"]).match_text("body^5", "harmful chemical", 3).to_pl()
# The matched words will be enclosed in <em> and </em>
```

---

### fusion

```python
table_object.fusion(method, topn, fusion_params = None)
```

Creates a reranking expression for multiple retrieval ways to identify the top n closest rows.

:::tip NOTE
To display your query results, you must chain this method with `output(columns)`, which specifies the columns to output, and a method such as `to_pl()`, `to_df()`, or `to_arrow()` to format the query results.
:::

#### Parameters

##### method: `str`, *Required*

A non-empty string indicating the reranking methods to use:

- `"rrf"`: [Reciprocal rank fusion](https://plg.uwaterloo.ca/~gvcormac/cormacksigir09-rrf.pdf)  
  RRF is a method for combining multiple result sets with varying relevance indicators into a single result set. It requires no tuning, and the relevance indicators need not be related to achieve high-quality results. RRF is particularly useful when you are uncertain of the relative importance of each retrieval way.  
  RRF uses the following formula to calculate the score for ranking each document:  

  ```python
  score = 0.0
  for q in queries:
      if d in result(q):
          score += 1.0 / ( k + rank( result(q), d ) )
  return score

  # Where
  # k is the ranking constant,
  # q is a query in a set of queries,
  # d is a document in the result set of q,
  # result(q) is the result set of q, and
  # rank( result(q), d ) is the rank of d within the result(q), starting from 1.
  ```

- `"weighted_sum"`  
  The weighted sum approach assigns different weights to different retrieval ways, allowing you to emphasize specific ways. This is particularly useful when you are certain of each path's relative importance.  
- `"match_tensor"`  
  Infinity's tensor-based late interaction reranking approach.  

##### topn: `int`, *Required*

An integer indicating the number of the most relevant rows to retrieve.

##### fusion_params: `dict[str, Any]`, *Optional*

A dictionary representing additional options for the selected reranking method:

- **RRF-specific options**: *Optional*  
  Settings when employing RRF for reranking.  
  - `"rank_constant"`: The smoothing constant for RRF reranking, e.g., `{"rank_constant": 60}`. Defaults to `60`.

- **weighted_sum-specific options**: *Optional*  
  Settings when employing Weighted Sum for reranking.  
  - `"weights"`: Specifies the weight for each retrieval way. For example, `{"weights": "1,2,0.5"}` sets weights of `1`, `2`, and `0.5` for the first, second, and third retrieval ways, respectively. The default weight of each retrieval way is `1.0`. If `"weight"` is not specified, all retrieval ways will be assigned the default weight of `1.0`.

- **match_tensor-specific options**: *Optional*  
  Settings when employing match_tensor for reranking.
  - `"field"`: The name of the tensor column for reranking.
  - `"query_tensor"`: The tensor data to compare against. This should be provided as a list of lists or a two-dimensional NumPy
    array of numerical values.
  - `"element_type"`: The element data type of the query tensor. Usually `"float"`.

#### Returns

- Success: An `infinity.remote_thrift.table.RemoteTable` object in client-server mode.
- Failure: `InfinityException`
  - `error_code`: `int` A non-zero value indicating a specific error condition.
  - `error_msg`: `str` A message providing additional details about the error.

#### Examples

The following code snippets illustrate the use of fused reranking in a three-way retrieval.

##### Use RRF for reranking

```python {6}
from infinity.common import SparseVector
table_object.output(["num", "body", "vec", "sparse_column", "year", "tensor", "_score"])
            .match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "cosine", 3)
            .match_sparse("sparse_column", SparseVector([0, 20, 80], [1.0, 2.0, 3.0]), "ip", 3)
            .match_text("body", "blooms", 10)
            .filter("year < 2024")
            .fusion("rrf", 2)
            .to_pl()
```

```python {6}
from infinity.common import SparseVector
table_object.output(["num", "body", "vec", "sparse_column", "year", "tensor", "_score"])
            .match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "cosine", 3)
            .match_sparse("sparse_column", SparseVector([0, 20, 80], [1.0, 2.0, 3.0]), "ip", 3)
            .match_text("body", "blooms", 10)
            .filter("year < 2024")
            .fusion("rrf", 2, {"rank_constant": 30})
            .to_pl()
```

##### Use Weighted Sum for reranking

```python {6}
from infinity.common import SparseVector
table_object.output(["num", "body", "vec", "sparse_column", "year", "tensor", "_score"])
            .match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "cosine", 3)
            .match_sparse("sparse_column", SparseVector([0, 20, 80], [1.0, 2.0, 3.0]), "ip", 3)
            .match_text("body", "blooms", 10)
            .filter("year < 2024")
            .fusion("weighted_sum", 2, {"weights": "1,2,0.5"})
            .to_pl()
```

##### Use tensor reranking

```python {8}
from infinity.common import SparseVector
table_object.output(["num", "body", "vec", "sparse_column", "year", "tensor", "_score"])
            .match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "cosine", 3)
            .match_sparse("sparse_column", SparseVector([0, 20, 80], [1.0, 2.0, 3.0]), "ip", 3)
            .match_text("body", "blooms", 10)
            .filter("year < 2024")
            .fusion("match_tensor", 2, {"field": "tensor", "element_type": "float", "query_tensor": [[0.0, -10.0, 0.0, 0.7], [9.2, 45.6, -55.8, 3.5]]})
            .to_pl()
```

---

### to_result


```python
table_object.to_result()
```

Returns the query result as a tuple.

:::tip NOTE
Call `to_result()` in a chain after (not necessarily "immediately after") `output(columns)` on the same table object.
:::

:::caution NOTE
We recommend calling `to_df()`, `to_pl()`, or `to_arrow()` to format your results.
:::

#### Returns 

A `tuple[dict[str, list[Any]], dict[str, Any]], {}` object

### to_df

```python
table_object.to_df()
```

Returns the query result as a tuple consisting of a pandas DataFrame and a dict.

:::tip NOTE
Call `to_df()` in a chain after (not necessarily "immediately after") `output(columns)` on the same table object.
:::

#### Returns

A `tuple[pandas.DataFrame, {}]` object

#### Examples

```python
# Format columns "c1" and C2" of the current table into a pandas DataFrame
res, extra_res = table_object.output(["c1", "c2"]).to_df()
```

### to_pl

```python
table_object.to_pl()
```

Returns the query result as a tuple consisting of a Polars DataFrame and a dict.

:::tip NOTE
Call `to_pl()` in a chain after (not necessarily "immediately after") `output(columns)` on the same table object.
:::

#### Returns

A `tuple[polas.DataFrame, {}]` object.

#### Examples

```python
# Format a vector search result into a Polars DataFrame. 
res, extra_res = table_object.output(["*"]).match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "ip", 10).to_pl()
```

### to_arrow

```python
table_object.to_arrow()
```

Returns the query result as a tuple consisting of an Apache Arrow Table and a dict.

:::tip NOTE
Call `to_arrow()` in a chain after (not necessarily "immediately after") `output(columns)` on the same table object.
:::

#### Returns

A `tuple[pyarrow.Table, {}]` object.

#### Examples

```python
# Format the current table object into an Apache Arrow Table. 
res, extra_result = table_object.output(["*"]).filter("score >= 90").to_arrow()
```

---
