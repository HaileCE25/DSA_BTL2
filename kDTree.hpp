#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct kDTreeNode
{
    vector<int> data;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }
    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
private:
    int k;
    kDTreeNode *root;

    void deleteNode(kDTreeNode *node) {
        if (node != nullptr){
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
        }
    }

    kDTreeNode* copyNode(kDTreeNode *node) {
        if (!node) {
        return nullptr;
        }
        kDTreeNode* newNode = new kDTreeNode(node->data);
        newNode->left = copyNode(node->left);
        newNode->right = copyNode(node->right);
        return newNode;
    }

    void printPoint(const kDTreeNode *node, ostringstream& oss) const {
        if (node) {
            oss << "(";
            for (int i = 0; i < k - 1; ++i) {
                oss << node->data[i] << ", ";
            }
            oss << node->data[k - 1] << ") ";
        }
    }

    string Inorder(const kDTreeNode *node) const {
        if (!node) return "";
        std::ostringstream oss;
        oss << Inorder(node->left);
        printPoint(node, oss);
        oss << Inorder(node->right);
        return oss.str();
    }

    string Preorder(const kDTreeNode *node) const {
        if (!node) return "";
        std::ostringstream oss;
        printPoint(node, oss);
        oss << Preorder(node->left);
        oss << Preorder(node->right);
        return oss.str();
    }

    string Postorder(const kDTreeNode *node) const {
        if (!node) return "";
        std::ostringstream oss;
        oss << Postorder(node->left);
        oss << Postorder(node->right);
        printPoint(node, oss);
        return oss.str();
    }

    int getHeight(const kDTreeNode *node) const {
        if (node == nullptr){
            return 0;
        }
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return max(leftHeight, rightHeight) + 1;
    }

    int countNode(const kDTreeNode *node) const {
        if (node == nullptr) {
            return 0;
        }
        int countLeft = countNode(node->left);
        int countRight = countNode(node->right);
        return countLeft + countRight + 1;
    }

    int countLeaf(const kDTreeNode *node) const {
        if (node == nullptr) {
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr) {
            return 1;
        }
        int leafLeft = countLeaf(node->left);
        int leafRight = countLeaf(node->right);
        return leafLeft + leafRight + 1;
    }

    kDTreeNode* insertNode(kDTreeNode *node, const vector<int>& point, int depth) {
        if (node == nullptr) {
            return new kDTreeNode(point);
        }
        if (point == node->data) {
            return node;
        }
        int dim = depth % k;
        if (point[dim] < node->data[dim]) {
            node->left = insertNode(node->left, point, depth + 1);
        } else {
            node->right = insertNode(node->right, point, depth + 1);
        }
        return node;
    }

    kDTreeNode* findMin(kDTreeNode *node, int dim, int depth) {
        if (!node) {
            return nullptr;
        }
        kDTreeNode* minNode = node;
        int currentDim = depth % k;
        if (currentDim == dim && node->left) {
            kDTreeNode* minLeft = findMin(node->left, dim, depth + 1);
            if (minLeft && minLeft->data[dim] < minNode->data[dim])
                minNode = minLeft;
        }
        if (node->left || node->right) {
            kDTreeNode* minChild = findMin(node->left ? node->left : node->right, dim , depth + 1);
            if (minChild && minChild->data[dim] < minNode->data[dim])
            minNode = minChild;
        }
        return minNode;
    }

    kDTreeNode* removeNode(kDTreeNode *node, const vector<int>& point, int depth) {
        if (!node) {
            return nullptr;
        }
        int dim = depth % k;
        if (node->data == point){
            if (!node->left && !node->right){
                delete node;
                return nullptr;
            }
            if (node->right) {
                kDTreeNode* minRight = findMin(node->right, dim, 0);
                node->data = minRight->data;
                node->right = removeNode(node->right, minRight->data, depth + 1);
            } else if (node->left) {
            kDTreeNode* minLeft = findMin(node->left, dim, 0);
            node->data = minLeft->data;
            node->left = removeNode(node->left, minLeft->data, depth + 1);
            }
            return node;
        }
        else {
            if (point[dim] < node->data[dim])
                node->left = removeNode(node->left, point, depth + 1);
                node->right = removeNode(node->right, point, depth + 1);
        }
        return node;
    }

    bool searchNode(kDTreeNode* node, const vector<int> &point, int depth) const {
        if (node == nullptr)
            return false;
        if (node->data == point)
            return true;
        int dim = depth % k;
        if (point[dim] < node->data[dim])
            return searchNode(node->left, point, depth + 1);
        return searchNode(node->right, point, depth + 1);
    }

    void quickSort(vector<vector<int>> &arr, int left, int right, int dim) {
        if (left >= right)
            return;
        int i = left, j = right;
        vector<int> pivot = arr[(left + right) / 2];
        while (i <= j) {
            while (arr[i][dim] < pivot[dim])
                i++;
            while (arr[j][dim] > pivot[dim])
                j--;
            if (i <= j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        if (left < j)
            quickSort(arr, left, j, dim);
        if (i < right)
            quickSort(arr, i, right, dim);
    }

    kDTreeNode* buildTree(kDTreeNode* node, const vector<vector<int>> &pointList, int depth) {
        if (pointList.empty())
            return nullptr;
        int dim = depth % k;
        vector<vector<int>> sortedPoints = pointList;
        quickSort(sortedPoints, 0, sortedPoints.size() - 1, dim);
        int medianIndex = (sortedPoints.size() - 1) / 2;
        while (medianIndex > 0 && sortedPoints[medianIndex - 1][dim] == sortedPoints[medianIndex][dim]) {
            medianIndex--;
        }
        vector<int> medianPoint = sortedPoints[medianIndex];
        node = new kDTreeNode(medianPoint);
        vector<vector<int>> leftPoints(sortedPoints.begin(), sortedPoints.begin() + medianIndex);
        vector<vector<int>> rightPoints(sortedPoints.begin() + medianIndex + 1, sortedPoints.end());
        node->left = buildTree(node->left, leftPoints, depth + 1);
        node->right = buildTree(node->right, rightPoints, depth + 1);
        return node;
    }

    double distance(const vector<int>& a, const vector<int>& b) const {
        double distance = 0;
        for (int i = 0; i < k; ++i) {
            distance += pow(a[i] - b[i], 2);
        }
        return sqrt(distance);
    }

    kDTreeNode* nearestNeighbour(kDTreeNode* node, const vector<int>& target, kDTreeNode *&best, int depth) {
        if (!node)
            return nullptr;
        int dim = depth % k;
        kDTreeNode* nearestNode = (target[dim] < node->data[dim]) ? node->left : node->right;
        best = nearestNeighbour(nearestNode, target, best, depth + 1);
        if (!best) 
            best = node;
        double d = distance(target, node->data);
        double R = distance(target, best->data);
        if (d < R)
            best = node;
        double splitDistance = abs(target[dim] - node->data[dim]);
        kDTreeNode* otherNode = (nearestNode == node->left) ? node->right : node->left;
        if (otherNode && splitDistance < R) {
            kDTreeNode* newBest = nullptr;
            otherNode = nearestNeighbour(otherNode, target, newBest, depth + 1);
            if (newBest && distance(target, newBest->data) < distance(target, best->data))
            best = newBest;
        }
        return best;
    }

public:
    kDTree(int k = 2);
    ~kDTree();

    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);

    void inorderTraversal() const;
    void preorderTraversal() const;
    void postorderTraversal() const;
    int height() const;
    int nodeCount() const;
    int leafCount() const;

    void insert(const vector<int> &point);
    void remove(const vector<int> &point);
    bool search(const vector<int> &point);
    void buildTree(const vector<vector<int>> &pointList);
    void nearestNeighbour(const vector<int> &target, kDTreeNode *&best);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset y_train;
    int numClasses;
    kDTree tree;

public:
    kNN(int k = 5);
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
};

// Please add more or modify as needed
