import numpy as np
import matplotlib.pyplot as plt

class bayesian_model_v2:

	def __init__(self, train_data, train_labels):

		self.num_images = train_data.shape[0]
		self.num_labels = 10
		self.priors = self.get_priors(train_labels)
		self.model = self.trian_model_v2(train_data, train_labels, row_feat, col_feat)

	def trian_model_v2(self, train_data, train_labels, row_feat, col_feat, disjoint):
		'''
		INPUT: train_data: (np.array) - (images, rows, cols)
			   train_labels: (np.array) - (labels per image)
			   row_feat: int - num of rows per feature
			   col_feat: int - num of cols per feature
			   disjoint: bool - check if disjoint or overlapping features are desired
		OUTPUT: model: (np.array) - (labels, row, cols)
		'''

		print("Training the model.... ")

		constant = 10

		self.values_per_feat = 2**(row_feat*col_feat)

		if(disjoint):
			self.total_row_feats = 32/row_feat
			self.total_col_feats = 32/col_feat
		else:
			self.total_row_feats = 

		model = np.zeros((self.num_labels, self.total_row_feats, self.total_col_feats))

		num_per_class = self.priors * self.num_images

		for image in np.arange(self.num_images):
			for i in np.arange(self.total_row_feats):
				for j in np.arange(self.total_col_feats):
					model[train_labels[image], i, ] = self.convert_to_value(train_data[image, i, ])

		smoothed_model = self.apply_laplace_smoothing(model, constant, num_per_class)

		for label in np.arange(self.num_labels):
			for row in np.arange(self.total_row_feats):
				for col in np.arange(self.total_col_feats):

		for label in np.arange(self.num_labels):
			smoothed_model[label,:,:] = smoothed_model[label,:,:]/ (num_per_class[label][0])

		print('Done!')
		return smoothed_model


	def convert_to_value(self, feature):
		'''
		INPUT: feature - np.array(row_feat x col_feat)
		OUTPUT: value - 0 - 2^(row_feat x col_feat)
		'''
		feature_array = feature.flatten()
		rev_feature_array = feature_array[::-1]
		rev_feature_array_list = rev_feature_array.tolist()
		rev_str = ''.join(map(str,numList))
		return int(rev_str, 2)











