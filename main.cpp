#include <algorithm>
#include <iostream>
#include <vector>

// 定义一个v3的向量
struct Vector3 {
    /* data */
    float x, y, z;

    /* function */
    Vector3(float x_, float y_, float z_)
        : x(x_), y(y_), z(z_) {}
    Vector3 operator-(const Vector3& v3);
    Vector3 operator+(const Vector3& v3);
    float& operator[](int index);
};

Vector3 Vector3::operator-(const Vector3& v3) {
    return Vector3(
        this->x - v3.x,
        this->y - v3.y,
        this->z - v3.z);
}

Vector3 Vector3::operator+(const Vector3& v3) {
    return Vector3(
        this->x + v3.x,
        this->y + v3.y,
        this->z + v3.z);
}

float& Vector3::operator[](int index) {
    if (index == 0)
        return x;
    else if (index == 1)
        return y;
    else if (index == 2)
        return z;
    else {
        throw std::out_of_range("Index out of range in Vector3::operator[]. Index should be 0, 1, 2.");
    }
}

// 定义一个AABB包围盒
struct AABB {
    /* data */
    Vector3 min, max;

    /* function */
    AABB(const Vector3& min_, const Vector3& max_)
        : min(min_), max(max_) {}
};

// 定义一个三角形结构
struct Triangle {
    /* data */
    Vector3 vertices[3];

    /* function */
    AABB boundingBox() const {
        Vector3 min(
            std::min({vertices[0].x, vertices[1].x, vertices[2].x}),
            std::min({vertices[0].y, vertices[1].y, vertices[2].y}),
            std::min({vertices[0].z, vertices[1].z, vertices[2].z}));
        Vector3 max(
            std::max({vertices[0].x, vertices[1].x, vertices[2].x}),
            std::max({vertices[0].y, vertices[1].y, vertices[2].y}),
            std::max({vertices[0].z, vertices[1].z, vertices[2].z}));
        return AABB(min, max);
    }
};

// 定义BVH结点
struct BVHNode {
    /* data */
    AABB boundingBox;
    BVHNode* leftChild;
    BVHNode* rightChild;
    std::vector<Triangle> triangles;

    /* function */
    BVHNode(const AABB& bbox)
        : boundingBox(bbox), leftChild(nullptr), rightChild(nullptr) {}
};

// 构建BVH
BVHNode* buildBVH(std::vector<Triangle>& triangles) {
    // 空三角判断
    if (triangles.empty())
        return nullptr;

    // 计算包围盒
    AABB bbox = triangles[0].boundingBox();
    for (auto tri : triangles) {
        AABB triBox = tri.boundingBox();
        bbox.min = Vector3(
            std::min(bbox.min.x, triBox.min.x),
            std::min(bbox.min.y, triBox.min.y),
            std::min(bbox.min.z, triBox.min.z));
        bbox.max = Vector3(
            std::max(bbox.max.x, triBox.max.x),
            std::max(bbox.max.y, triBox.max.y),
            std::max(bbox.max.z, triBox.max.z));
    }

    // 如果三角形数量小于等于1， 则创建叶子结点
    if (triangles.size() <= 1) {
        BVHNode* leaf = new BVHNode(bbox);
        leaf->triangles = std::move(triangles);
        return leaf;
    }

    // 找到最长的边
    Vector3 extent = bbox.max - bbox.min;
    int longestAxis = (extent.x > extent.y && extent.x > extent.z) ? 0 : (extent.y > extent.z) ? 1
                                                                                               : 2;
    float splitPosition = (bbox.min[longestAxis] + bbox.max[longestAxis]) * 0.5f;

    // 分割三角形
    std::vector<Triangle> leftTriangles, rightTriangles;
    for (auto tri : triangles) {
        AABB triBox = tri.boundingBox();
        float triCenter = (triBox.min[longestAxis] + triBox.max[longestAxis]) * 0.5f;

        if (triCenter <= splitPosition) {
            leftTriangles.push_back(tri);
        } else {
            rightTriangles.push_back(tri);
        }
    }

    // 递归构建左右子树
    BVHNode* node = new BVHNode(bbox);
    node->leftChild = buildBVH(leftTriangles);
    node->rightChild = buildBVH(rightTriangles);
    return node;
}

// 显示三角形和包围盒信息
void displayTriangle(const Triangle& tri) {
    std::cout << "Triangle vertices:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "  Vertex " << i << ": (" << tri.vertices[i].x << ", " << tri.vertices[i].y << ", " << tri.vertices[i].z << ")" << std::endl;
    }
    AABB bbox = tri.boundingBox();
    std::cout << "Bounding box: " << std::endl;
    std::cout << "  Min: (" << bbox.min.x << ", " << bbox.min.y << ", " << bbox.min.z << ")" << std::endl;
    std::cout << "  Max: (" << bbox.max.x << ", " << bbox.max.y << ", " << bbox.max.z << ")" << std::endl;
}

// 显示BVH节点和包围盒信息
void displayBVHNode(BVHNode* node, int depth = 0) {
    if (node == nullptr)
        return;
    std::cout << "BVHNode at depth " << depth << ":" << std::endl;
    std::cout << "Bounding box: " << std::endl;
    std::cout << "  Min: (" << node->boundingBox.min.x << ", " << node->boundingBox.min.y << ", " << node->boundingBox.min.z << ")" << std::endl;
    std::cout << "  Max: (" << node->boundingBox.max.x << ", " << node->boundingBox.max.y << ", " << node->boundingBox.max.z << ")" << std::endl;
    std::cout << "Number of triangles: " << node->triangles.size() << std::endl;
    if (node->leftChild != nullptr || node->rightChild != nullptr) {
        std::cout << "Left child:" << std::endl;
        displayBVHNode(node->leftChild, depth + 1);
        std::cout << "Right child:" << std::endl;
        displayBVHNode(node->rightChild, depth + 1);
    }
}

int main(int argc, char* argv[]) {
    // 构建一些三角形（硬编码测试数据）
    std::vector<Triangle> triangles;
    triangles.push_back({{Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0)}});
    triangles.push_back({{Vector3(1, 1, 1), Vector3(2, 2, 2), Vector3(3, 3, 3)}});
    triangles.push_back({{Vector3(-1, -1, -1), Vector3(-2, -2, -2), Vector3(-3, -3, -3)}});

    // 显示三角形信息和包围盒信息
    for (int i = 0; i < triangles.size(); ++i) {
        std::cout << "Triangle " << i + 1 << ":" << std::endl;
        displayTriangle(triangles[i]);
        std::cout << std::endl;
    }

    // 构建BVH
    BVHNode* root = buildBVH(triangles);

    // 显示BVH节点信息和包围盒信息
    displayBVHNode(root);

    // 在这里你可以进行光线追踪等操作

    // 清理内存
    delete root;

    return 0;
}