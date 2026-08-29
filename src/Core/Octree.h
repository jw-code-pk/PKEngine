#pragma once

#include <Ogre.h>

template <typename T> class Octree {
public:
  struct OctreeNode {
    const T *Data;
    const Ogre::Vector3 Origin;
  };

  Octree(const Ogre::Vector3 &Origin, const Ogre::Vector3 &Extents,
         const int &NodesPerSplit = 4) {
    m_Origin = Origin;
    m_Extents = Extents;
    m_MaxNodeCount = NodesPerSplit;
  }

  ~Octree() {
    if (m_HasSplit) {
      for (int i = 0; i < 8; i++) {
        delete m_Children[i];
      }
    }
  }

  bool TryAdd(const OctreeNode &Node) {
    if (!HasOverlap(Node.Origin)) {
      return false;
    }

    if (m_HasSplit) {
      AddToChildren(Node);
    } else if (m_Data.size() == m_MaxNodeCount) {
      Split();
      AddToChildren(Node);
    } else {
      m_Data.push_back(Node);
    }

    return true;
  }

  void FetchAll(const Ogre::Vector3 &Point, const Ogre::Vector3 &Extents,
                std::vector<OctreeNode> &Results) const {
    if (!HasOverlap(Point, Extents)) {
      return;
    }

    if (m_HasSplit) {
      for (const auto child : m_Children) {
        child->FetchAll(Results);
      }
    } else if (m_Data.size() > 0) {
      Results.insert(Results.end(), m_Data.begin(), m_Data.end());
    }
  }

protected:
  bool HasOverlap(const Ogre::Vector3 &Point) {
    return std::abs(Point.x - m_Origin.x) <= m_Extents.x &&
           std::abs(Point.y - m_Origin.y) <= m_Extents.y &&
           std::abs(Point.z - m_Origin.z) <= m_Extents.z;
  }

  bool HasOverlap(const Ogre::Vector3 &Point, const Ogre::Vector3 &Extents) {
    return std::abs(Point.x - m_Origin.x) <= (Extents.x + m_Extents.x) &&
           std::abs(Point.y - m_Origin.y) <= (Extents.y + m_Extents.y) &&
           std::abs(Point.z - m_Origin.z) <= (Extents.z + m_Extents.z);
  }

  void Split() {
    const auto halfExtents = m_Extents * 0.5f;

    m_Children[0] = new Octree(
        m_Origin + Ogre::Vector3(halfExtents.x, halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[1] = new Octree(
        m_Origin + Ogre::Vector3(halfExtents.x, halfExtents.y, -halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[2] = new Octree(
        m_Origin + Ogre::Vector3(-halfExtents.x, halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[3] = new Octree(
        m_Origin + Ogre::Vector3(-halfExtents.x, halfExtents.y, -halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[4] = new Octree(
        m_Origin + Ogre::Vector3(halfExtents.x, -halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[5] = new Octree(
        m_Origin + Ogre::Vector3(halfExtents.x, -halfExtents.y, -halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[6] = new Octree(
        m_Origin + Ogre::Vector3(-halfExtents.x, -halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[7] =
        new Octree(m_Origin + Ogre::Vector3(-halfExtents.x, -halfExtents.y,
                                            -halfExtents.z),
                   halfExtents, m_MaxNodeCount);

    for (const auto &node : m_Data) {
      AddToChildren(node);
    }

    m_Data.clear();
    m_HasSplit = true;
  }

  void AddToChildren(const OctreeNode &Node) {
    for (auto child : m_Children) {
      if (child.TryAdd(Node)) {
        return;
      }
    }
  }

private:
  Ogre::Vector3 m_Origin;
  Ogre::Vector3 m_Extents;

  int m_MaxNodeCount;
  bool m_HasSplit;
  Octree *m_Children[8];
  std::vector<OctreeNode> m_Data;
};
