#include "maze.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>

maze_s::maze_s(maze_coordinate width, maze_coordinate height, size_t seed) {
    this->width  = width;
    this->height = height;
    this->seed   = seed;

    std::srand(seed);

    for (maze_coordinate x = 0; x < width; x++) {
        for (maze_coordinate y = 0; y < height; y++) {
            unvisited_cells.insert(std::make_tuple(x, y));
        }
    }

    for (maze_coordinate x = 0; x < width; x++) {
        for (maze_coordinate y = 0; y < height; y++) {
            maze_node n  = std::make_tuple(x, y);
            maze_node nx = std::make_tuple(x + 1, y);
            maze_node ny = std::make_tuple(x, y + 1);
            add_edge(n, nx);
            add_edge(n, ny);
            unvisited_cells.insert(n);
        }
    }

    // Generate maze
    maze_node current = {0, 0};
    unvisited_cells.erase(current);
    visited_cells.insert(current);
    while (!unvisited_cells.empty()) {
        auto neighbors = get_unvisited_neighbors(current);
        if (neighbors.empty()) {
            auto [visited, unvisited] = get_random_visited_cell_with_unvisited_neighbor();
            remove_edge(visited, unvisited);
            current = unvisited;
        } else {
            size_t random_index = std::rand() % neighbors.size();
            auto   neighbor     = neighbors[random_index];
            remove_edge(current, neighbor);
            current = neighbor;
        }

        unvisited_cells.erase(current);
        visited_cells.insert(current);
    }
}

std::vector<maze_node> maze_s::get_cell_neighbors(const maze_node& cell) const {
    std::vector<maze_node> neighbors;
    std::vector<maze_node> adjacencies = {
        { 0,  1},
        { 0, -1},
        { 1,  0},
        {-1,  0}
    };

    for (const auto& adjacency : adjacencies) {
        maze_coordinate x = std::get<0>(cell) + std::get<0>(adjacency);
        maze_coordinate y = std::get<1>(cell) + std::get<1>(adjacency);
        maze_node       n = std::make_tuple(x, y);
        if (is_valid_cell(n)) {
            neighbors.push_back(n);
        }
    }

    return neighbors;
}

std::vector<maze_node> maze_s::get_unvisited_neighbors(const maze_node& cell) const {
    std::vector<maze_node> unvisited;
    for (const auto& neighbor : get_cell_neighbors(cell)) {
        if (unvisited_cells.contains(neighbor)) {
            unvisited.push_back(neighbor);
        }
    }

    return unvisited;
}

std::tuple<maze_node, maze_node> maze_s::get_random_visited_cell_with_unvisited_neighbor() const {
    std::vector<std::tuple<maze_node, maze_node>> candidates;

    for (const auto& visited : visited_cells) {
        auto neighbors = get_cell_neighbors(visited);
        for (const auto& neighbor : neighbors) {
            if (unvisited_cells.contains(neighbor)) {
                candidates.push_back(std::make_tuple(visited, neighbor));
            }
        }
    }

    size_t index = std::rand() % candidates.size();
    return candidates[index];
}

bool maze_s::is_valid_cell(const maze_node& cell) const {
    auto [x, y] = cell;
    return x >= 0 && y >= 0 && x < width && y < height;
}

bool maze_s::has_edge(const maze_node& a, const maze_node& b) const {
    auto e1 = std::make_tuple(a, b);
    auto e2 = std::make_tuple(b, a);
    return edges.contains(e1) || edges.contains(e2);
}

void maze_s::add_edge(const maze_node& a, const maze_node& b) {
    if (!is_valid_cell(a) || !is_valid_cell(b))
        return;

    auto e = std::make_tuple(a, b);
    edges.insert(e);
}

void maze_s::remove_edge(const maze_node& a, const maze_node& b) {
    auto e1 = std::make_tuple(a, b);
    auto e2 = std::make_tuple(b, a);
    edges.erase(e1);
    edges.erase(e2);
}

bool is_edge_horizontal(const maze_edge& edge) {
    auto n1 = std::get<0>(edge);
    auto n2 = std::get<1>(edge);

    auto x1 = std::get<0>(n1);
    auto y1 = std::get<1>(n1);
    auto x2 = std::get<0>(n2);
    auto y2 = std::get<1>(n2);

    return x1 == x2 && y1 != y2;
}