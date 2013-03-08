#include "Stage.hpp"
#include <stdio.h>
#include <string.h>

/**
 * get Haar Cascade classifier from file
 */
int GetHaarCascade(const char* filename, vector<Stage>& Stages)
{
	FILE *fin;
	int size;
	unsigned int i, t, s, n_stages, n_trees;
	unsigned int stages, trees, value, flag;
	unsigned long total_features;
	fin = fopen(filename, "r");
	assert(fin);
	fscanf(fin, "%d%d", &size, &size);
	printf("Sample size %d x %d\n", size, size);
	fscanf(fin, "%d", &n_stages);
	printf("Total %d stages\n", n_stages);
	s = 1;
	while(!feof(fin) && s <= n_stages) // get each stage
	{
		Stage stage;
		fscanf(fin, "%d", &n_trees); // num of trees per stage
		printf("Stage %d num_tree %d ", s, n_trees);
		t = 1;
		while(t <= n_trees && !feof(fin)) // get each tree's feature
		{
			Rectangle rect;
			Feature feature;
			Tree tree;
			i = 0;
			flag = 0;
			trees = t;
			while(t==trees && i<7)
			{
				fscanf(fin, "%d%d%d", &stages, &trees, &value);
				assert(stages == s);
				assert(value == 1);
				if(i < 2)
				{
					fscanf(fin, "%d", &value);
					assert(value == i+1);
					fscanf(fin, "%d", &rect.x);
					fscanf(fin, "%d", &rect.y);
					fscanf(fin, "%d", &rect.width);
					fscanf(fin, "%d", &rect.height);
					fscanf(fin, "%lf", &rect.weight);
					feature.addRect(rect);
				}
				else if(i == 2)
				{
					fscanf(fin, "%d", &value);
					if(value == 0)
					{
						feature.tilted = 0;
						memset(&rect, 0, sizeof(Rectangle));
						feature.addRect(rect);
					}
					else if(value == 3) // exist 3rd rect
					{
						flag = 1;
						fscanf(fin, "%d", &rect.x);
						fscanf(fin, "%d", &rect.y);
						fscanf(fin, "%d", &rect.width);
						fscanf(fin, "%d", &rect.height);
						fscanf(fin, "%lf", &rect.weight);
						feature.addRect(rect);
					}
					else
						assert(0);
				}
				else
				{
					if(flag) i--;  // exist 3rd rect
					switch(i)
					{
						case 2: fscanf(fin, "%d", &feature.tilted);     break;
						case 3: fscanf(fin, "%lf", &feature.threshold); break;
						case 4: fscanf(fin, "%lf", &feature.left_val);  break;
						case 5: fscanf(fin, "%lf", &feature.right_val); break;
					}
					if(flag) i++;
					if(!flag && i==5) i++;
				}
				i++;
			}
			t++;
			feature.w = 20;
			feature.h = 20;
			feature.has_left_val = true;
			feature.has_right_val = true;
			assert(feature.nb_rects<=3);
			tree.addFeature(feature);
			stage.addTree(tree);
		}
		s++;
		assert(stage.trees.size() == trees);
		fscanf(fin, "%d %lf", &stages, &stage.threshold); // get threshold of stage
		printf("threshold %lf\n", stage.threshold);
		Stages.push_back(stage);
	}
	total_features = 0;
	for(i=0; i<Stages.size(); i++)
	{
		printf("%d\t", i+1);
		printf("%d\n", Stages[i].trees.size());
		total_features += Stages[i].trees.size();
	}
	printf("Total features %ld\n", total_features);
	return size;
}
