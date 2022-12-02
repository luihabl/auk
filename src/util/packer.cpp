#include "auk/util/packer.h"
#include <algorithm>

using namespace auk;

Packer::Node::Node(int _id, int _w, int _h)
{
    id = _id;
    w = _w;
    h = _h;
}

Packer::Packer(int w, int h)
{
    max_w = w; 
    max_h = h;
}

struct BNode
{
    int x = 0, y = 0;
    int w = 0, h = 0;
    
    bool used = false;
    BNode* right = nullptr;
    BNode* down = nullptr;

    BNode(int _x, int _y, int _w, int _h)
    {
        x = _x; y = _y; w = _w; h = _h;
    }

    ~BNode()
    {
        if(used)
        {
            delete right;
            delete down;
        }
        used = false;
        right = nullptr;
        down = nullptr;
    }
};

BNode* find_bin_tree(BNode* node, int w, int h)
{
    if(node->used)
    {
        BNode* node_right = find_bin_tree(node->right, w, h);
        return node_right ? node_right : find_bin_tree(node->down, w, h);
    }
    else if((w <= node->w) && (h <= node->h))
        return node;
    else
        return nullptr;
}

BNode* split_bin_tree(BNode* node, int w, int h)
{
    node->used = true;
    node->down  = new BNode(node->x,     node->y + h, node->w,     node->h - h);
    node->right = new BNode(node->x + w, node->y,     node->w - w, h);
    return node;
}

BNode* grow_right(BNode* root, const Packer::Node& r)
{
    auto* old_root = new BNode(*root);
    *old_root = *root;

    root->used = true;
    root->x = 0;
    root->y = 0;
    root->w = old_root->w + r.w;
    root->h = old_root->h;
    root->down = old_root;
    root->right = new BNode(old_root->w, 0, r.w, old_root->h);

    auto* node = find_bin_tree(root, r.w, r.h);
    if(node)
    {
        return split_bin_tree(node, r.w, r.h);
    }
    else
        return nullptr;
}

BNode* grow_down(BNode* root, const Packer::Node& r)
{
    auto* old_root = new BNode(*root);
    *old_root = *root;

    root->used = true;
    root->x = 0;
    root->y = 0;
    root->w = old_root->w;
    root->h = old_root->h + r.h;
    root->down = new BNode(0, old_root->h, old_root->w, r.h);
    root->right = old_root;

    auto* node = find_bin_tree(root, r.w, r.h);
    if(node)
    {
        return split_bin_tree(node, r.w, r.h);
    }
    else
        return nullptr;
}





BNode* grow_bin_tree(BNode* root, Packer::Node& r, int w_max, int h_max)
{
    bool can_grow_down  = (r.w <= root->w) && ((r.h + root->h) <= h_max);
    bool can_grow_right = (r.h <= root->h) && ((r.w + root->w) <= w_max);

    bool should_grow_right = can_grow_right && (root->h >= (root->w + r.w));
    bool should_grow_down  = can_grow_down  && (root->w >= (root->h + r.h));

    if (should_grow_right)
        return grow_right(root, r);
    else if(should_grow_down)
        return grow_down(root, r);

    if(can_grow_right)
        return grow_right(root, r);
    else if(can_grow_down)
        return grow_down(root, r);

    return nullptr;
}


BoolVec2 Packer::pack_bin_tree()
{
    auto* root = new BNode(0, 0, nodes[0].w, nodes[1].h);
    bool all_fit = true;
    bool none_fit = true;

    for (auto & r : nodes)
    {
        BNode* node = find_bin_tree(root, r.w, r.h);
        if(node)
        {
            r.x = node->x;
            r.y = node->y;
            r.packed = true;
            r.page = current_page;
            split_bin_tree(node, r.w, r.h);

            none_fit = false;
        }
        else
        {
            BNode* expanded_node = grow_bin_tree(root, r, max_w, max_h);
            if(expanded_node)
            {
                r.x = expanded_node->x;
                r.y = expanded_node->y;
                r.packed = true;
                r.page = current_page;

                none_fit = false;
            }
            else
            {
                r.packed = false;
                all_fit = false;
            }
        }
    }

    pages.push_back({root->w, root->h});

    delete root;
    return {all_fit, none_fit};
}


bool Packer::pack(bool paging)
{
    pages.clear();

    // Sort nodes by the max side
    std::sort(nodes.begin(), nodes.end(), [](const Node& r0, const Node& r1){
        return std::max(r0.w, r0.h) > std::max(r1.w, r1.h);
    });

    bool all_packed = false;
    bool ok = false;
    while(!ok)
    {
        auto res = pack_bin_tree();
        ok = res[0];

        all_packed = ok;

        if(!paging || res[1])
            break;

        current_page++;
    }

    return all_packed;
}

void Packer::add(int ref, int w, int h)
{
    nodes.emplace_back(ref, w, h);
}

void Packer::clear()
{
    nodes.clear();
    pages.clear();
}