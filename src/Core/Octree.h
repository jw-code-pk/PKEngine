#pragma once

#include <Ogre.h>
#include <algorithm>

template <typename T> class Octree {
public:
  struct OctreeNode {
    T Data;
    Ogre::Vector3 Origin;

    bool operator==(const OctreeNode &Other) {
      return (Data == Other.Data && Origin == Other.Origin);
    }
  };

  Octree(const Ogre::Vector3 &Origin, const Ogre::Vector3 &Extents,
         const int &NodesPerSplit = 4) {
    m_AAB = Ogre::AxisAlignedBox(Origin - Extents, Origin + Extents);
    m_MaxNodeCount = NodesPerSplit;
    m_HasSplit = false;
  }

  ~Octree() {
    if (m_HasSplit) {
      for (int i = 0; i < 8; i++) {
        delete m_Children[i];
      }
    }
  }

  bool TryAdd(const Ogre::Vector3 &Origin, T Data) {
    auto node = OctreeNode{.Data = Data, .Origin = Origin};
    return TryAdd(node);
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

  bool TryRemove(const Ogre::Vector3 &Origin, T Data) {
    auto node = OctreeNode{.Data = Data, .Origin = Origin};
    return TryRemove(node);
  }

  bool TryRemove(const OctreeNode &Node) {
    if (m_HasSplit) {
      for (int i = 0; i < 8; i++) {
        if (m_Children[i]->TryRemove(Node)) {
          return true;
        }
      }
    } else if (HasOverlap(Node.Origin)) {
      auto iter = std::find(m_Data.begin(), m_Data.end(), Node);

      if (iter != m_Data.end()) {
        m_Data.erase(iter);
        return true;
      }
    }

    return false;
  }

  void ClearAll() {
    m_Data.clear();

    if (m_HasSplit) {
      for (int i = 0; i < 8; i++) {
        auto child = m_Children[i];
        child->ClearAll();
      }
    }
  }

  void FetchAll(const Ogre::AxisAlignedBox &Area, std::set<T> &Results) const {
    if (!HasOverlap(Area)) {
      return;
    }

    if (m_HasSplit) {
      for (auto child : m_Children) {
        child->FetchAll(Area, Results);
      }
    } else if (m_Data.size() > 0) {
      // Results.insert(Results.end(), m_Data.begin(), m_Data.end());
      for (auto node : m_Data) {
        Results.insert(node.Data);
      }
    }
  }

  void GetChildren(std::vector<Octree<T> *> &Results) {
    if (!m_HasSplit) {
      return;
    }

    for (int i = 0; i < 8; i++) {
      auto child = m_Children[i];
      Results.push_back(child);
      child->GetChildren(Results);
    }
  }

  Ogre::AxisAlignedBox GetAAB() { return m_AAB; }

protected:
  bool HasOverlap(const Ogre::Vector3 &Point) const {
    return m_AAB.intersects(Point);
  }

  bool HasOverlap(const Ogre::AxisAlignedBox &Box) const {
    return m_AAB.intersects(Box);
  }

  void Split() {
    assert(!m_HasSplit && "Split should only be called once on an octree.");

    const auto origin = m_AAB.getCenter();
    const auto halfExtents = m_AAB.getHalfSize() * 0.5f;

    m_Children[0] = new Octree(
        origin + Ogre::Vector3(halfExtents.x, halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[1] = new Octree(
        origin + Ogre::Vector3(halfExtents.x, halfExtents.y, -halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[2] = new Octree(
        origin + Ogre::Vector3(-halfExtents.x, halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[3] = new Octree(
        origin + Ogre::Vector3(-halfExtents.x, halfExtents.y, -halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[4] = new Octree(
        origin + Ogre::Vector3(halfExtents.x, -halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[5] = new Octree(
        origin + Ogre::Vector3(halfExtents.x, -halfExtents.y, -halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[6] = new Octree(
        origin + Ogre::Vector3(-halfExtents.x, -halfExtents.y, halfExtents.z),
        halfExtents, m_MaxNodeCount);
    m_Children[7] = new Octree(
        origin + Ogre::Vector3(-halfExtents.x, -halfExtents.y, -halfExtents.z),
        halfExtents, m_MaxNodeCount);

    for (auto &node : m_Data) {
      AddToChildren(node);
    }

    m_Data.clear();
    m_HasSplit = true;
  }

  void AddToChildren(const OctreeNode &Node) {
    for (int i = 0; i < 8; i++) {
      auto child = m_Children[i];

      if (child->TryAdd(Node)) {
        return;
      }
    }
  }

private:
  Ogre::AxisAlignedBox m_AAB;

  int m_MaxNodeCount;
  bool m_HasSplit;
  Octree *m_Children[8];
  std::vector<OctreeNode> m_Data;
};
