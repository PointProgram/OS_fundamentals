<h1> Cache-memory management</h1>

## General info
A model that implements the algorithm for searching and replacing data in the memory cache for randomly mapping data to the cache. Data alignment of the main and cache memory is performed according to the principle of end-to-end recording (write through).


## Technologies
Project is created with:
* C++
* gcc: version 6.3.0

	
## Setup
git clone and run a program with the following makefile:

<code>$ make</code>

## Data Caching

Computer memory is a hierarchy of storage devices (SDs) that differ in average data access time, volume and cost of storing 1 bit of information. The foundation of this pyramid is the memory on hard drives, but the access time to the disk is calculated in milliseconds. RAM has an access time of 10-20 ns (from several megabytes to several gigabytes). RAM (tens to hundreds of kilobytes) – 8 ns. The internal registers of the processor are several tens of bytes with an access time of 2-3 ns.
Cache memory or simply cache is a way of joint functioning of two types of memory, which differ in access time and cost of data storage.
Dynamic copying of the most frequently used information from "slow" RAM to fast RAM allows, on the one hand, to reduce the average data access time and, on the other hand, to save more expensive high-speed memory. A feature
caching is that the system does not require any external information about the intensity of data usage. Neither users nor programs accept

no participation in moving data from a storage device of one type to a memory device of another type - all this is done automatically by system means.
Cache is often called not only a method of organizing two types of RAM, but also one of the devices - "fast RAM". It is more expensive and relatively small in volume, in contrast to the "slow" RAM - RAM. If caching is used to reduce the average access time to the operating system
memory, then more expensive and fast-acting static memory is used as a cache.
If caching is used by the input-output system to speed up access to data stored on the disk, buffers implemented in RAM play the role of memory cache.
Virtual memory can also be considered as a special case
caching

# The working principle of cache memory

The content of the cache memory is a set of records about all the data elements loaded into it from the main memory. Each entry includes:
- data item;
- the address of the element in the main memory;
- additional information for the implementation of the algorithm, which includes a sign of modification and a sign of data validity.

With each access to the main memory by physical address, the contents of the memory cache are viewed in order to determine the presence of the necessary data there.

The cache memory is non-addressable, so the search for the required data is carried out by content - by the value of the address field in the RAM taken from the request.

Next, two development options are possible:
- the data is found in the memory cache, i.e. a cache-hit occurs, it is read and transmitted to the source of the request;
 
- the required data is missing, that is, a cache miss has occurred, they are read from the main memory, transmitted to the source of the request and simultaneously copied to the cache memory.

The efficiency of caching depends on the probability of hitting the cache.

If we denote the probability of a cache hit by p, and the access time to the main memory by t1, the access time to the cache by t2, then according to the full probability formula, the average access time will be equal to:
    t = t2p + t1(1-p)
    If p=1, the access time is t2.
    The probability of detecting data in the cache depends on various factors: the volume of the cache;
    - amount of cached memory;
    - the algorithm for replacing data in the cache;
    - features of the executable program, etc.

In practice, the percentage of hits turns out to be quite high - about 90%. Such a percentage is determined by the presence of objective properties in the data, namely spatial and temporal locality.

Spatial locality. If an address has been contacted, it is highly likely that neighboring addresses will be contacted in the near future.

Temporal locality. If there has been a call to some address, then the next call to the same address is very likely to happen in the near future.

Since during the execution of the program there is a very high probability that commands are selected from memory one by one from neighboring cells, it makes sense to load an entire program fragment into the cache. Similarly, with arrays of data. During operation, the contents of the memory cache are constantly updated. Ejecting data means either simply declaring some area of ​​the cache memory free (resetting the validity bit) or, in addition, copying data into the main
memory, if they have been modified.
The presence of two copies of data in the computer - in the main memory and in the cache - creates a problem of data consistency.
There are two approaches to solving this problem:
   - write through. When requesting the main memory (including when writing), the cache is viewed. If there is no data at the requested address, writing is performed only in the main memory. If the data is in the cache, the write is made to both the cache and the memory.
   - write back. The cache is checked and, if there is no data there, the entry is made to the main memory. Otherwise, the write is made only to the cache. At the same time, a sign of modification is established. When data is pushed from the cache, this data will be overwritten in the main memory.

# Ways to map the main memory to the CACHE

Algorithms for finding and replacing data in the cache depend on how the main memory is mapped to the cache.
Two display schemes are used:
    - random display;
    - deterministic mapping.

With random mapping, a RAM element can be placed anywhere in the cache memory (along with its RAM address). The search for information is carried out at this address. These are the procedures for simple sorting of addresses with large time costs. Therefore, the so-called associative search is used, when the comparison is not performed sequentially with each entry in the cache, but in parallel with all its entries. The sign by which the comparison is performed is called a tag. In this case, the tag is the address of data in RAM. The hardware implementation of such a search significantly increases the cost of cache memory. Therefore, this method is used to ensure a high percentage of hits with a small amount of cache memory.
In caches based on random mapping, old data is pushed out only when there is no free space in the memory cache. The selection of data for unloading is based on the same principles as when replacing pages (long time no accesses, the least number of accesses, etc.).
With the deterministic method of mapping, any element of the main memory is mapped to the same cache memory location. In this case, the cache memory is divided into lines, each of which is designed to store one record of one data element and has its own number.
A "one-to-many" correspondence is established between cache memory line numbers and RAM addresses: several (quite a few) RAM addresses correspond to one line number. A simple allocation of several bits from the RAM address, which are interpreted as a line number of the cache memory, can be used for display. This mapping is called direct. For example, the cache is designed for 1024 entries (1024 lines). Then any RAM address can be mapped to a cache memory address by simple division of 10 binary bits When searching for data in the cache, fast direct access to the record is used by the line number obtained from the RAM address from the request.
When searching for data in the cache, fast direct access to the record is used by the line number obtained from the RAM address from the request. In addition, an additional check is performed to match the tag with the corresponding part of the address from the request. If the tag matches, a cache hit is detected. Otherwise, a cache miss is detected, the data is read from the OP and copied to the cache.
In many modern processors, the cache memory is built on the basis of a combination of these two approaches. In a mixed approach, an arbitrary RAM address is mapped not to one cache memory address (as is typical for direct mapping), and not to any cache memory address (as in random mapping), but to some group of addresses . All groups are numbered. The search in the cache is carried out first by the group number obtained from the RAM address from the request, and then within the group by associative review of all group records to match the older parts of the RAM addresses.
In case of a miss, the data is copied to any free address from the specified group. If there are no free addresses in the group, data is squeezed out.

In case of a miss, the data is copied to any free address from the specified one
groups If there are no free addresses in the group, data is squeezed out. Since there are several candidates for uploading (all records from this group), the replacement algorithm can take into account the intensity of data access and thus increase the probability of hits in the future.
As we have seen, the cache is viewed only for the purpose of matching the contents of the cache and the main memory. If a miss occurs, write requests do not cause any changes to the cache. In some implementations of the memory cache, when there is no data in the cache, it is copied there from the main memory, regardless of whether a read or write request is performed.
According to the described logic of the memory cache, when a request occurs, the cache is first looked at, and then, if a miss occurred, the main memory is accessed. However, another robotic cache scheme is often implemented: the search in the cache and main memory starts simultaneously, then, depending on the result of browsing the cache, the operation in the main memory either continues or is interrupted.