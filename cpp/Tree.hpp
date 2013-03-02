#ifndef __TREE_H__
#define __TREE_H__

#include <vector>
#include <math.h>
#include "Feature.hpp"
#include "Image.hpp"
using namespace std;

/** 
 * A binary tree for detection. At each node of the tree,
 * a test (feature) is done on the window. Depending on the result,
 * either the left or right child of the node is selected and so on.
 * When the process reaches a leaf, the corresponding value is returned.
 */
typedef struct Tree
{
	vector<Feature> features;

	void addFeature(Feature f)
	{
		features.push_back(f);
	}

	float getVal(Image& grayImage, Image& squares, int i, int j, float scale)
	{
		Feature& cur_node = features[0];
		while(true)
		{
			/* Compute the feature to see if we should go to the left or right child on the node.*/
			int where = cur_node.getLeftOrRight(grayImage, squares, i, j, scale);
			if(where == LEFT)
			{
				/* If the left child has a value, return it.*/
				if(cur_node.has_left_val)
				{
					return cur_node.left_val;
				}
				else
				{
					/* Else move to the left child node. */
					cur_node = features[cur_node.left_node];
				}
			}
			else
			{
				/* If the right child has a value, return it.*/
				if(cur_node.has_right_val)
				{
					return cur_node.right_val;
				}
				else
				{
					/* Else move to the right child node. */
					cur_node = features[cur_node.right_node];
				}
			}
		}
	}
}Tree;

#endif
