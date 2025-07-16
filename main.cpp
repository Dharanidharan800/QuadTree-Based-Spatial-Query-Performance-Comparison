#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <algorithm>
using namespace std;

struct Point {
    double x, y;
    Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}
};

struct QuadTreeNode {
    double x_min, x_max, y_min, y_max;
    vector<Point> points;
    QuadTreeNode *nw, *ne, *sw, *se;
    int capacity;
    bool divided;

    QuadTreeNode(double xmin, double xmax, double ymin, double ymax, int cap = 50)
        : x_min(xmin), x_max(xmax), y_min(ymin), y_max(ymax), capacity(cap),
          nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr), divided(false) {}

    bool contains(Point p) {
        return (p.x >= x_min && p.x <= x_max && p.y >= y_min && p.y <= y_max);
    }

    void subdivide() {
        double midX = (x_min + x_max) / 2;
        double midY = (y_min + y_max) / 2;
        nw = new QuadTreeNode(x_min, midX, midY, y_max, capacity);
        ne = new QuadTreeNode(midX, x_max, midY, y_max, capacity);
        sw = new QuadTreeNode(x_min, midX, y_min, midY, capacity);
        se = new QuadTreeNode(midX, x_max, y_min, midY, capacity);
        divided = true;
    }

    void insert(Point p) {
        if (!contains(p)) return;

        if (points.size() < capacity) {
            points.push_back(p);
        } else {
            if (!divided) subdivide();
            nw->insert(p);
            ne->insert(p);
            sw->insert(p);
            se->insert(p);
        }
    }

    void query(double qx_min, double qx_max, double qy_min, double qy_max, vector<Point>& found) {
        if (x_max < qx_min || x_min > qx_max || y_max < qy_min || y_min > qy_max)
            return;

        for (Point& p : points) {
            if (p.x >= qx_min && p.x <= qx_max && p.y >= qy_min && p.y <= qy_max) {
                found.push_back(p);
            }
        }

        if (divided) {
            nw->query(qx_min, qx_max, qy_min, qy_max, found);
            ne->query(qx_min, qx_max, qy_min, qy_max, found);
            sw->query(qx_min, qx_max, qy_min, qy_max, found);
            se->query(qx_min, qx_max, qy_min, qy_max, found);
        }
    }

    void saveToFile(ofstream& out) {
        out << x_min << " " << x_max << " " << y_min << " " << y_max << " " << points.size() << "\n";
        for (auto& p : points)
            out << p.x << " " << p.y << "\n";
        if (divided) {
            nw->saveToFile(out);
            ne->saveToFile(out);
            sw->saveToFile(out);
            se->saveToFile(out);
        }
    }
};

int main() {
    srand(time(0));
    vector<Point> data;

    cout << "Generating 1 million points...\n";
    for (int i = 0; i < 1000000; ++i) {
        double x = (rand() / (double)RAND_MAX) * 1000;
        double y = (rand() / (double)RAND_MAX) * 1000;
        data.emplace_back(x, y);
    }

    // Sort by x for binary search
    vector<Point> data_sorted = data;
    sort(data_sorted.begin(), data_sorted.end(), [](Point a, Point b) {
        return a.x < b.x;
    });

    cout << "Building Quad-Tree...\n";
    QuadTreeNode tree(0, 1000, 0, 1000);
    for (auto& p : data)
        tree.insert(p);

    ofstream outFile("quadtree_output.txt");
    tree.saveToFile(outFile);
    outFile.close();
    cout << "QuadTree saved to 'quadtree_output.txt'\n";

    cout << "Running 20 queries (linear vs quad-tree vs binary)...\n";
    
//Custom Queries 
/*
    vector<tuple<double, double, double, double>> customQueries = {
        {123, 242, 244, 351}, {227, 355, 253, 417}, {633, 864, 774, 916}, {0, 71, 16, 99},
        {131, 240, 246, 359}, {231, 354, 261, 421}, {634, 857, 773, 920}, {2, 72, 11, 90},
        {125, 239, 250, 345}, {232, 362, 252, 410}, {645, 870, 778, 917}, {7, 65, 22, 93},
        {124, 241, 249, 346}, {228, 359, 256, 411}, {643, 861, 781, 912}, {6, 67, 15, 87},
        {126, 237, 243, 353}, {225, 347, 250, 418}, {648, 867, 785, 910}, {4, 68, 19, 95}
    };
    
    for (int i = 0; i < customQueries.size(); ++i) {
        auto [x1, x2, y1, y2] = customQueries[i];
        cout << "\nCustom Query " << i + 1 << ": x1=" << x1 << ", y1=" << y1
             << ", x2=" << x2 << ", y2=" << y2 << "\n";
    
        // Linear search
        auto start1 = chrono::high_resolution_clock::now();
        int linear_count = 0;
        for (auto& p : data)
            if (p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2)
                linear_count++;
        auto end1 = chrono::high_resolution_clock::now();
    
        // QuadTree search
        vector<Point> found;
        auto start2 = chrono::high_resolution_clock::now();
        tree.query(x1, x2, y1, y2, found);
        auto end2 = chrono::high_resolution_clock::now();
    
        // Binary + Y-filter search
        auto start3 = chrono::high_resolution_clock::now();
        auto low = lower_bound(data_sorted.begin(), data_sorted.end(), x1,
            [](const Point& p, double val) { return p.x < val; });
    
        auto high = upper_bound(data_sorted.begin(), data_sorted.end(), x2,
            [](double val, const Point& p) { return val < p.x; });
    
        int binary_count = 0;
        for (auto it = low; it != high; ++it)
            if (it->y >= y1 && it->y <= y2)
                binary_count++;
        auto end3 = chrono::high_resolution_clock::now();
    
        // Print results
        cout << "Linear     = " << linear_count
             << ", Time = " << chrono::duration_cast<chrono::microseconds>(end1 - start1).count() << " µs\n";
    
        cout << "QuadTree   = " << found.size()
             << ", Time = " << chrono::duration_cast<chrono::microseconds>(end2 - start2).count() << " µs\n";
    
        cout << "Binary+Y   = " << binary_count
             << ", Time = " << chrono::duration_cast<chrono::microseconds>(end3 - start3).count() << " µs\n";
    }
 */
 
 //Random Queries
    for (int i = 0; i < 20; ++i) {
        double x1 = rand() % 950;
        double y1 = rand() % 950;
        double x2 = x1 + 50;
        double y2 = y1 + 50;
        cout << "\nQuery " << i + 1 << ": x1=" << x1 << ", y1=" << y1 << ", x2=" << x2 << ", y2=" << y2 << "\n";

        // LS
        auto start1 = chrono::high_resolution_clock::now();
        int linear_count = 0;
        for (auto& p : data)
            if (p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2)
                linear_count++;
        auto end1 = chrono::high_resolution_clock::now();

        // QS
        vector<Point> found;
        auto start2 = chrono::high_resolution_clock::now();
        tree.query(x1, x2, y1, y2, found);
        auto end2 = chrono::high_resolution_clock::now();

        // BS
        auto start3 = chrono::high_resolution_clock::now();
        auto low = lower_bound(data_sorted.begin(), data_sorted.end(), x1,
            [](const Point& p, double val) { return p.x < val; });

        auto high = upper_bound(data_sorted.begin(), data_sorted.end(), x2,
            [](double val, const Point& p) { return val < p.x; });

        int binary_count = 0;
        for (auto it = low; it != high; ++it) {
            if (it->y >= y1 && it->y <= y2)
                binary_count++;
        }
        auto end3 = chrono::high_resolution_clock::now();

        cout << "Linear: " << linear_count << ", Time = "
             << chrono::duration_cast<chrono::microseconds>(end1 - start1).count() << " µs\n";
        cout << "QuadTree: " << found.size() << ", Time = "
             << chrono::duration_cast<chrono::microseconds>(end2 - start2).count() << " µs\n";
        cout << "Binary+Y-Filter: " << binary_count << ", Time = "
             << chrono::duration_cast<chrono::microseconds>(end3 - start3).count() << " µs\n";
    }

    return 0;
}
