
#include <cstddef>
#include <set>
#include <tuple>
#include <vector>

using maze_coordinate = short;
using maze_node       = std::tuple<maze_coordinate, maze_coordinate>;
using maze_edge       = std::tuple<maze_node, maze_node>;

struct maze_s {
   private:
    std::set<maze_node> unvisited_cells;
    maze_node           get_random_unvisited_cell() const;
    bool                get_random_unvisited_neighbor(const maze_node& cell, maze_node* neighbor) const;

   public:
    std::set<maze_edge> edges;
    maze_coordinate     width;
    maze_coordinate     height;
    size_t              seed;

    maze_node start;
    maze_node end;

    maze_s(maze_coordinate width, maze_coordinate height, size_t seed);

    bool has_edge(const maze_node& a, const maze_node& b) const;
    void add_edge(const maze_node& a, const maze_node& b);
    void remove_edge(const maze_node& a, const maze_node& b);
};

bool is_edge_horizontal(const maze_edge& edge);