
#include <cstddef>
#include <set>
#include <tuple>
#include <vector>

using maze_coordinate = short;
using maze_node       = std::tuple<maze_coordinate, maze_coordinate>;
using maze_edge       = std::tuple<maze_node, maze_node>;

struct maze_s {
   private:
    std::set<maze_node>              unvisited_cells;
    std::set<maze_node>              visited_cells;
    bool                             is_valid_cell(const maze_node& cell) const;
    std::vector<maze_node>           get_cell_neighbors(const maze_node& cell) const;
    std::vector<maze_node>           get_unvisited_neighbors(const maze_node& cell) const;
    std::tuple<maze_node, maze_node> get_random_visited_cell_with_unvisited_neighbor() const;

   public:
    std::set<maze_edge> edges;
    maze_coordinate     width;
    maze_coordinate     height;
    size_t              seed;

    maze_s(maze_coordinate width, maze_coordinate height, size_t seed);

    bool has_edge(const maze_node& a, const maze_node& b) const;
    void add_edge(const maze_node& a, const maze_node& b);
    void remove_edge(const maze_node& a, const maze_node& b);
};

bool is_edge_horizontal(const maze_edge& edge);