#include "map.hpp"

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


