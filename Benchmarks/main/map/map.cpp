#include "map.hpp"

constexpr const char *tag = "unordered_map";

_map umap_1 = 
{
    {1, "foo"},
    {2, "bar"},
    {3, "baz"}
};

_map umap_empty{};

_map umap_2 = 
{
    {7, "fiz"},
    {8, "buz"},
    {9, "jaz"}
};

_map umap_large = 
{
    {1, "foo"},
    {2, "bar"},
    {3, "baz"},
    {4, "pat"},
    {5, "cat"},
    {6, "bat"},
    {10, "joe"},
    {11, "jim"},
    {12, "syd"},
};

#if USE_ETL
    std::pair<int, _string_sm> mapElem = {11, "box"};
#else
    std::pair<int, _string> mapElem = {11, "box"};
#endif

void
map_benchmark(uint32_t cycles)
{
    CYCLE_GET_COUNT (
        _map m{},
        cycles, 
        "unordered_map<int, string>: _map m{}"
    );

    CYCLE_GET_COUNT (
        _map m{umap_1},
        cycles, 
        "unordered_map<int, string>: _map m{umap_1}"
    );

    CYCLE_GET_COUNT (
        _map m(umap_1.begin(), umap_1.end()),
        cycles, 
        "unordered_map<int, string>: _map m(umap_1.begin(), umap_1.end())"
    );

    CYCLE_GET_COUNT_RETURN (
        umap_1[2], 
        cycles, 
        "unordered_map<int, string>: operator[]"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        umap_1.insert(umap_1.begin(), std::make_pair(11, "box")),
        umap_1.erase(11),
        cycles, 
        "unordered_map<int, string>: insert(map.begin, std::make_pair)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        {},
        umap_1.insert(std::make_pair(11, "box")),
        umap_1.erase(11),
        cycles, 
        "unordered_map<int, string>: insert(std::make_pair)"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        umap_1.insert(mapElem),
        umap_1.erase(mapElem.first),
        cycles, 
        "unordered_map<int, string>: insert(mapElem)"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        umap_large.insert(umap_2.begin(), umap_2.end()),
        {for (const auto& [key, _] : umap_2) { umap_large.erase(key); }},
        cycles, 
        "unordered_map<int, string>: insert(begin, end)"
    );

    CYCLE_GET_COUNT_MUTATE (
        auto elem = umap_1.find(1); \
        mapElem = std::make_pair(elem->first, elem->second),
        umap_1.erase(mapElem.first), 
        umap_1.insert(mapElem), 
        cycles, 
        "unordered_map<int, string>: erase(key)"
    );

    CYCLE_GET_COUNT (
        umap_large.find(13),
        cycles,
        "unordered_map<int, string>: find(key)"
    );
    
    CYCLE_GET_COUNT_RETURN(
        umap_1.size(),
        cycles,
        "unordered_map: size()"
    );

    CYCLE_GET_COUNT_RETURN(
        umap_1.empty(),
        cycles,
        "unordered_map: empty()"
    );
    
    CYCLE_GET_COUNT_RETURN(
        umap_1.at(2),
        cycles,
        "unordered_map: at(key)"
    );

    CYCLE_GET_COUNT_RETURN(
        umap_1.count(2),
        cycles,
        "unordered_map: count(key)"
    );

    CYCLE_GET_COUNT_RETURN(
        umap_1.count(100),
        cycles,
        "unordered_map: count(nonexistent key)"
    );
    
    CYCLE_GET_COUNT_RETURN(
        umap_large.bucket_count(),
        cycles,
        "unordered_map: bucket_count()"
    );

    CYCLE_GET_COUNT_RETURN(
        umap_large.load_factor(),
        cycles,
        "unordered_map: load_factor()"
    );
    
    CYCLE_GET_COUNT_RETURN(
        (umap_1 == umap_1),
        cycles,
        "unordered_map: equality comparison (==)"
    );
    
    CYCLE_GET_COUNT_MUTATE(
        {},
        umap_1[1] = "modified",
        umap_1[1] = "foo",
        cycles,
        "unordered_map: operator[] overwrite"
    );

    ESP_LOGI(tag, "DONE\n");
}

