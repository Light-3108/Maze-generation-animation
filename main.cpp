/* Generate maze using backtracking algorithm animation*/
#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#define SIDE_RIGHT 0
#define SIDE_DOWN 1
#define SIDE_LEFT 2
#define SIDE_TOP 3

#define GRID_WIDTH 20
#define GRID_HEIGHT 15

#define NODE_SIZE 50


//mazeHelper.h
struct Node {
    bool walls[4] = { true, true, true, true };
    bool visited = false;
    bool paths[4] = { false, false, false, false };
    int n_visited = 0;
};

void drawNode(sf::RenderWindow& window, Node nodeList[], int col, int row, bool isCurrent = false) {
    sf::RectangleShape rect(sf::Vector2f(NODE_SIZE, NODE_SIZE));
    rect.setPosition(col * NODE_SIZE, row * NODE_SIZE);

    if (row == 0 && col == 0) {
        rect.setFillColor(sf::Color::Red);
    }
    else if (row == GRID_HEIGHT - 1 && col == GRID_WIDTH - 1) {
        rect.setFillColor(sf::Color::Green);
    }
    else {
        rect.setFillColor(sf::Color::Blue);
    }

    window.draw(rect);

    float half = NODE_SIZE / 2.f;
    sf::Vector2f center(col * NODE_SIZE + half, row * NODE_SIZE + half);
    sf::CircleShape circle(2.f);
    circle.setFillColor(isCurrent ? sf::Color::Red : sf::Color::Green);
    circle.setPosition(center.x - 1.f, center.y - 1.f);
    window.draw(circle);

    if (nodeList[col + row * GRID_WIDTH].walls[SIDE_TOP]) {
        sf::RectangleShape line(sf::Vector2f(NODE_SIZE, 1.f));
        line.setPosition(col * NODE_SIZE, row * NODE_SIZE);
        window.draw(line);
    }
    if (nodeList[col + row * GRID_WIDTH].walls[SIDE_LEFT]) {
        sf::RectangleShape line(sf::Vector2f(1.f, NODE_SIZE));
        line.setPosition(col * NODE_SIZE, row * NODE_SIZE);
        window.draw(line);
    }
    if (nodeList[col + row * GRID_WIDTH].walls[SIDE_DOWN]) {
        sf::RectangleShape line(sf::Vector2f(NODE_SIZE, 1.f));
        line.setPosition(col * NODE_SIZE, (row + 1) * NODE_SIZE);
        window.draw(line);
    }
    if (nodeList[col + row * GRID_WIDTH].walls[SIDE_RIGHT]) {
        sf::RectangleShape line(sf::Vector2f(1.f, NODE_SIZE));
        line.setPosition((col + 1) * NODE_SIZE, row * NODE_SIZE);
        window.draw(line);
    }
}


void drawMaze(sf::RenderWindow& window, Node nodeList[]) {
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            drawNode(window, nodeList, j, i);
        }
    }
}

bool indexIsValid(int col, int row) {
    return (col >= 0 && col < GRID_WIDTH&& row >= 0 && row < GRID_HEIGHT);
}

int nextCol(int cur_col, int side) {
    switch (side) {
    case SIDE_RIGHT: return cur_col + 1;
    case SIDE_LEFT: return cur_col - 1;
    default: return cur_col;
    }
}

int nextRow(int cur_row, int side) {
    switch (side) {
    case SIDE_DOWN: return cur_row + 1;
    case SIDE_TOP: return cur_row - 1;
    default: return cur_row;
    }
}

void setWall(Node nodeList[], int col, int row, int side, bool state) {
    nodeList[col + row * GRID_WIDTH].walls[side] = state;
    int next_col = nextCol(col, side);
    int next_row = nextRow(row, side);
    if (indexIsValid(next_col, next_row)) {
        int opposite = (side + 2) % 4;
        nodeList[next_col + next_row * GRID_WIDTH].walls[opposite] = state;
    }
}

bool hasUnvisitedNodes(Node nodeList[]) {
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
        if (!nodeList[i].visited) return true;
    }
    return false;
}

bool hasUnvisitedNeighbor(Node nodeList[], int col, int row) {
    int count = 0;
    if (indexIsValid(col + 1, row) && !nodeList[col + 1 + row * GRID_WIDTH].visited) count++;
    if (indexIsValid(col - 1, row) && !nodeList[col - 1 + row * GRID_WIDTH].visited) count++;
    if (indexIsValid(col, row + 1) && !nodeList[col + (row + 1) * GRID_WIDTH].visited) count++;
    if (indexIsValid(col, row - 1) && !nodeList[col + (row - 1) * GRID_WIDTH].visited) count++;
    return count > 0;
}

//End of mazehelper.h
//start of the backtracking algorithm//                                           

void drawNodePath(sf::RenderWindow& window, Node nodeList[], int col, int row);
int main()
{
    srand(time(NULL));   // making generated maze unique everytime.  time(null) -- time in sec since epoch

    Node nodeList[GRID_WIDTH * GRID_HEIGHT];

    // beginning of backtracking algorithm (to generate the maze)
    int cur_col = rand() % GRID_WIDTH;
    int cur_row = rand() % GRID_HEIGHT;
    nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;
    std::stack<int> stack_col;
    std::stack<int> stack_row;

    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * NODE_SIZE, GRID_HEIGHT * NODE_SIZE), "Maze");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        while (hasUnvisitedNodes(nodeList))
        {
            if (hasUnvisitedNeighbor(nodeList, cur_col, cur_row))
            {
                int nextSide;
                int next_col, next_row;
                do {
                    nextSide = rand() % 4;
                    next_col = nextCol(cur_col, nextSide);
                    next_row = nextRow(cur_row, nextSide);
                } while (!indexIsValid(next_col, next_row) ||
                    nodeList[next_col + next_row * GRID_WIDTH].visited);
                stack_col.push(cur_col);
                stack_row.push(cur_row);
                setWall(nodeList, cur_col, cur_row, nextSide, false);
                cur_col = next_col;
                cur_row = next_row;
                nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;
            }
            else
            {
                cur_col = stack_col.top();
                cur_row = stack_row.top();
                stack_col.pop();
                stack_row.pop();
            }
        }
        drawMaze(window, nodeList);
        drawNode(window, nodeList, cur_col, cur_row, true);
        window.display();
    }
    // end of backtracking algorithm
    }
}
