#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

kDTree::kDTree(int k) : k(k), root(nullptr)
{
}

kDTree::~kDTree()
{
    deleteNode(root);
}

const kDTree &kDTree::operator=(const kDTree &other)
{
    if (this == &other){
        return *this;
    }
    this->k = other.k;
    deleteNode(root);
    this->root = copyNode(other.root);
    return *this;
}

kDTree::kDTree(const kDTree &other) : k(other.k), root(nullptr)
{
    root = copyNode(other.root);
}

void kDTree::inorderTraversal() const {
    cout << Inorder(root);
}

void kDTree::preorderTraversal() const {
    cout << Preorder(root);
}

void kDTree::postorderTraversal() const {
    cout << Postorder(root);
}

int kDTree::height() const
{
    return getHeight(root);
}

int kDTree::nodeCount() const
{
    return countNode(root);
}

int kDTree::leafCount() const
{
    return countLeaf(root);
}

void kDTree::insert(const vector<int> &point)
{
    if (point.size() != k)
        return;
    root = insertNode(root, point, 0);
}

void kDTree::remove(const vector<int> &point)
{
    if (point.size() != k)
        return;
    root = removeNode(root, point, 0);
}

bool kDTree::search(const vector<int> &point)
{
    return searchNode(root, point, 0);
}

void kDTree::buildTree(const vector<vector<int>>& pointList)
{
    root = buildTree(root, pointList, 0);
}

void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best)
{
    best = nearestNeighbour(root, target, best, 0);
    best = new kDTreeNode(best->data);
}

void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList) {
    if (!root) return;
    vector<kDTreeNode*> nearestNeighbors;
    for (int i = 0; i < k; ++i) {
        kDTreeNode* best = nullptr;
        nearestNeighbour(target, best);
        if (!best) break;
        nearestNeighbors.push_back(best);
        remove(best->data);
    }
    for (kDTreeNode* node : nearestNeighbors) {
        bestList.push_back(new kDTreeNode(node->data));
        insert(node->data);
    }
    for (kDTreeNode* node : nearestNeighbors) {
        delete node;
    }
}


///////CLASS KNN///////////
kNN::kNN(int k) : k(k)
{
}

void kNN::fit(Dataset &X_train, Dataset &y_train)
{
    this->X_train = X_train;
    this->y_train = y_train;
    tree = kDTree(X_train.data.front().size());
    vector<vector<int>> train_data;
    for (const auto& row : X_train.data) {
        vector<int> train_data(row.begin(), row.end());
        tree.insert(train_data);
    }
}

Dataset kNN::predict(Dataset &X_test) {
    Dataset y_pred;
    y_pred.columnName.push_back("label");
    for (auto test_sample : X_test.data) {
        vector<int> target(test_sample.begin(), test_sample.end());
        vector<kDTreeNode*> bestList;
        tree.kNearestNeighbour(target, k, bestList);
        int labelCount[10] = {0};
        for (auto it : bestList) {
            auto it_y = y_train.data.begin();
            for (auto it_x : X_train.data) {
                vector<int> train_sample(it_x.begin(), it_x.end());
                if (it->data == train_sample) {
                    int label = (*it_y).front();
                    labelCount[label]++;
                    break; 
                }
                ++it_y;
            }
        }
        int maxCount = 0;
        int predictedLabel = 0;
        for (int label = 0; label < 10; ++label) {
            if (labelCount[label] > maxCount) {
                maxCount = labelCount[label];
                predictedLabel = label;
            }
        }
        y_pred.data.push_back({predictedLabel});
        for (auto node : bestList) {
            delete node;
        }
    }
    return y_pred;
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred) {
    if (y_test.data.empty() || y_test.data.front().size() != y_pred.data.front().size()) {
        return 0.0;
    }
    int correct = 0;
    auto true_it = y_test.data.begin();
    auto pred_it = y_pred.data.begin();
    for (true_it ; true_it != y_test.data.end(); ++true_it, ++pred_it) {
        if ((*true_it).front() == (*pred_it).front()) {
            correct++;
        }
    }
    return static_cast<double>(correct) / y_test.data.size();
}

