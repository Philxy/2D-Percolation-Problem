#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

// prints grid to console
void printGrid(vector<vector<bool>> grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    };
}

// occupies squares of a grid given a probability
void randomize(double p, vector<vector<bool>> &grid)
{
    std::srand(time(NULL));
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            double rand = ((double)std::rand() / (double)RAND_MAX);
            grid[i][j] = rand < p;
        }
    };
    return;
}

struct cell
{
    int representant = 0;
    bool occupied = false;
    bool visited = false;
};

// initializes the cells based on the grid with random occupation
void initCells(vector<vector<cell>> &cells, vector<vector<bool>> &grid)
{
    int n = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cells[i][j].occupied = grid[i][j];
        }
    };
    return;
}

// finds the largest cluster by assigning an equivalency class representing a cluster to each occupied cell
void rasterScan(vector<vector<cell>> &cells)
{
    int largest_label = 0;
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells.size(); j++)
        {
            cell &curr_cell = cells[i][j];
            // Skip unoccupied cells
            if (!curr_cell.occupied)
            {
                continue;
            }
            // Find cells right and below current cell
            cell &right_cell = (j != cells.size() - 1) ? cells[i][j + 1] : cells[i][0];
            cell &below_cell = (i != cells.size() - 1) ? cells[i + 1][j] : cells[0][j];
            // if cell is isolated create new representant group
            if (!curr_cell.visited && !right_cell.occupied && !below_cell.occupied)
            {
                largest_label += 1;
                curr_cell.representant = largest_label;
            }
            // if current cell is new and all neighbours are unvisited exist
            else if (!curr_cell.visited && !right_cell.visited && !below_cell.visited && (right_cell.occupied || below_cell.occupied))
            {
                largest_label += 1;
                curr_cell.representant = largest_label;
                if (right_cell.occupied)
                {
                    right_cell.representant = curr_cell.representant;
                    right_cell.visited = true;
                }
                if (below_cell.occupied)
                {
                    below_cell.representant = curr_cell.representant;
                    below_cell.visited = true;
                }
            }
            // if current cell is new but right already visited: assign curr cell to right aquevalency class
            else if (!curr_cell.visited && right_cell.visited)
            {
                curr_cell.representant = right_cell.representant;
            }
            else if (curr_cell.visited)
            {
                if (right_cell.visited)
                {
                    for (int row = 0; row < cells.size(); row++)
                    {
                        for (int col = 0; col < cells.size(); col++)
                        {
                            if (!cells[row][col].visited || !cells[row][col].occupied)
                            {
                                continue;
                            }
                            if (cells[row][col].representant == right_cell.representant)
                            {
                                cells[row][col].representant = curr_cell.representant;
                            }
                        }
                    }
                }
                else if (right_cell.occupied)
                {
                    right_cell.representant = curr_cell.representant;
                    right_cell.visited = true;
                }
                if (below_cell.occupied)
                {
                    below_cell.representant = curr_cell.representant;
                    below_cell.visited = true;
                }
            }
            curr_cell.visited = true;
        };
    };
    return;
}

// print the representant of each cell to the console
void printCells(vector<vector<cell>> cells)
{
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            cout << cells[i][j].representant << " ";
        }
        cout << endl;
    };
}

// Finds the largest cluster and returns its label
int labelLargestCluster(vector<vector<cell>> cells)
{
    vector<int> labels(cells.size() * cells.size(), 0);
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            if (cells[i][j].occupied)
            {
                labels[cells[i][j].representant] += 1;
            }
        }
    };
    int currMax = 0;
    for (int i : labels)
    {
        currMax = (i > currMax) ? i : currMax;
    }
    for (int i = 0; i < labels.size(); i++)
    {
        if (labels[i] == currMax)
        {
            return i;
        }
    }
    return -1;
}

// assigns the value 2 to any cell which is part of the largest cluster, 1 to any other occupied cell
// and 0 to unoccupied cells.
void largestCluster(vector<vector<cell>> &cells)
{
    int label = labelLargestCluster(cells);
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            if (!cells[i][j].occupied)
            {
                cells[i][j].representant = 0;
            }
            else
            {
                if (cells[i][j].representant != label)
                {
                    cells[i][j].representant = 1;
                }
                else
                {
                    cells[i][j].representant = 2;
                }
            }
        }
    }
}

// writes the grid of cells to a file given a name
void writeClustersToFile(vector<vector<cell>> cells, string name)
{
    ofstream file;
    file.open(name + ".txt");
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            file << cells[i][j].representant << " ";
        }
        file << "\n";
    }
    file.close();
}

// writes the grid to a file
void writeGridToFile(vector<vector<bool>> grid, string name)
{
    ofstream file;
    file.open(name + ".txt");
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            file << grid[i][j] << " ";
        }
        file << "\n";
    }
    file.close();
}

int main()
{
    // initialize grid
    int gridSize = 50;
    double occupationProbability = 0.65;
    vector<vector<cell>> cells(gridSize, vector<cell>(gridSize));
    vector<vector<bool>> grid(gridSize, vector<bool>(gridSize));
    randomize(occupationProbability, grid);
    initCells(cells, grid);

    //printGrid(grid);
    cout << " " << endl;
    rasterScan(cells);
    //printCells(cells);
    largestCluster(cells);
    writeClustersToFile(cells, "largestCluster " + std::to_string(occupationProbability) + " " + std::to_string(gridSize) + " ");

    return 0;
}