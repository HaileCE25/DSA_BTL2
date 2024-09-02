#include "kDTree.hpp"

void tc1()
{
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    dataset.printHead(10, 10);
    std::cout << endl;
    dataset.printTail(10, 10);
    std::cout << endl;
    int nRows, nCols;
    dataset.getShape(nRows, nCols);
    std::cout << "Shape: " << nRows << "x" << nCols << endl;
}

void tc2()
{
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);
    feature.getShape(nRows, nCols);
    std::cout << "Feature shape: " << nRows << "x" << nCols << endl;
    label.getShape(nRows, nCols);
    std::cout << "Label shape: " << nRows << "x" << nCols << endl;
}

void tc3()
{
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    X_train.getShape(nRows, nCols);
    std::cout << "X_train shape: " << nRows << "x" << nCols << endl;
    X_test.getShape(nRows, nCols);
    std::cout << "X_test shape: " << nRows << "x" << nCols << endl;
    y_train.getShape(nRows, nCols);
    std::cout << "y_train shape: " << nRows << "x" << nCols << endl;
    y_test.getShape(nRows, nCols);
    std::cout << "y_test shape: " << nRows << "x" << nCols << endl;
}

void tc4()
{
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    knn.fit(X_train, y_train);
    Dataset y_pred = knn.predict(X_test);

    std::cout << "y_pred" << endl;
    y_pred.printHead(10, 10);
    std::cout << endl;
    std::cout << "y_test" << endl;
    y_test.printHead(10, 10);
    std::cout << endl;
}

void tc5()
{

    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    knn.fit(X_train, y_train);
    Dataset y_pred = knn.predict(X_test);
    double accuracy = knn.score(y_test, y_pred);
    std::cout << "Accuracy: " << accuracy << endl;
}

void tc6() {
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({8, 7});
    tree.insert({8, 1});
    tree.insert({9, 4});
    tree.insert({3, 5});
    tree.preorderTraversal();
    cout << endl;
    tree.inorderTraversal();
    cout << endl;
    tree.postorderTraversal();
    cout << endl;
    tree.height();
    cout << endl;
    tree.nodeCount();
    cout << endl;
    tree.leafCount();
}
void tc7(){
    kDTree tree(2);
    vector<vector<int>> pointList = {{5, 6}, {2, 2}, {7, 3}, {2, 8}, {8, 7}, {8, 1}, {9, 4}, {3, 5}};
    tree.buildTree(pointList);
    tree.preorderTraversal();
}

void tc8(){
    kDTree tree(2);
    vector<vector<int>> pointList = {{5, 6}, {2, 2}, {7, 3}, {2, 8}, {8, 7}, {8, 1}, {9, 4}, {3, 5}};
    tree.buildTree(pointList);
    tree.preorderTraversal();
}

void tc9(){
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({3, 5});
    tree.insert({8, 2});
    tree.insert({8, 7});
    tree.insert({9, 2});
    tree.insert({9, 5});
    kDTreeNode *best = nullptr;
    tree.nearestNeighbour({9, 3}, best);
    cout << "Nearest neighbour of (9, 3) is " << *best << endl;
}

void tc10(){
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({3, 5});
    tree.insert({8, 2});
    tree.insert({8, 7});
    tree.insert({9, 2});
    tree.insert({9, 5});
    vector<kDTreeNode *> bestList;
    tree.kNearestNeighbour({9, 3}, 5, bestList);
    cout << "5 Nearest neighbour of (9, 3) are: ";
    for (auto node : bestList)
    {
        cout << *node << " ";
    }
}

void tc11(){
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    knn.fit(X_train, y_train);
    Dataset y_pred = knn.predict(X_test);

    std::cout << "y_pred" << endl;
    y_pred.printHead(10, 10);
    std::cout << endl;
    std::cout << "y_test" << endl;
    y_test.printHead(10, 10);
    std::cout << endl;
}
void tc12() {
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    knn.fit(X_train, y_train);
    Dataset y_pred = knn.predict(X_test);
    double accuracy = knn.score(y_test, y_pred);
    std::cout << "Accuracy: " << accuracy << endl;
}
int main(int argc, const char *argv[])
{
    // tc11();
    // cout << endl;
    // tc12();
    tc8();
    return 0;
}