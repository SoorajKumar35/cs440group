import numpy as np
import matplotlib.pyplot as plt
import pickle as pk

class bayesian_model_v2:

	def __init__(self, train_data, train_labels, row_feat, col_feat, constant ,disjoint = True):

		self.num_images = train_data.shape[0]
		self.image_rows = train_data.shape[1]
		self.image_cols = train_data.shape[1]
		self.num_labels = 10
		self.priors = self.get_priors(train_labels)
		self.feat_row_len = row_feat
		self.feat_col_len = col_feat
		self.values_per_feat = 2**(row_feat * col_feat)
		self.constant = constant
		self.model = self.train_model_v2(train_data, train_labels, disjoint)

	def train_model_v2(self, train_data, train_labels, disjoint):
		'''
		INPUT: train_data: (np.array) - (images, rows, cols)
			   train_labels: (np.array) - (labels per image)
			   row_feat: int - num of rows per feature
			   col_feat: int - num of cols per feature
			   disjoint: bool - check if disjoint or overlapping features are desired
		OUTPUT: model: (np.array) - (labels, row, cols)
		'''

		# print("Training the model.... ")

		constant = self.constant

		if(disjoint):
			self.total_row_features = 32/self.feat_row_len
			self.total_col_features = 32/self.feat_col_len
			self.total_feats_per_image = self.total_row_features * self.total_col_features

			image_features, model = self.convert_to_features(train_data, disjoint, train_labels)

			# print("Created the image features: ")
			# print("Created the model: ")

			smoothed_model = self.apply_laplace_smoothing(model,constant)

			# print("Smoothed the model: ")
			return smoothed_model
		else:
			self.total_row_features = 0
			self.total_col_features = 0
			for i in np.arange(self.image_rows):
				if(i + self.feat_row_len <= self.image_rows):
					self.total_row_features += 1
				if(i + self.feat_col_len <= self.image_cols):
					self.total_col_features += 1
			self.total_feats_per_image = self.total_row_features * self.total_col_features

			image_features, model = self.convert_to_features(train_data, disjoint, train_labels)

			# print("Created the image features: ")
			# print("Created the model: ")

			smoothed_model = self.apply_laplace_smoothing(model, constant)

			# print("Smoothed the model: ")
			return smoothed_model

	def get_priors(self, train_labels):
		"""
		INPUT: train_labels: np.array - (num_samples, 1) - The labels of the training set each corresponding to one observation
		OUTPUT: priors: np.array - (num_labels, 1) - The priors for each class
		"""
		priors = np.zeros((self.num_labels, 1))
		for i in np.arange(self.num_images):
			priors[train_labels[i][0]] += 1
		self.examples_per_class = priors
		priors = priors * (1 / self.num_images)

		return priors

	def model_training(self, image_features, train_labels):
		model = np.zeros((self.num_labels, int(self.total_feats_per_image), self.values_per_feat))
		for image in np.arange(image_features.shape[0]):
			# print(image)
			for feat in np.arange(self.total_feats_per_image):
				for val in np.arange(self.values_per_feat):
					model[train_labels[image], int(feat), val] += image_features[image, int(feat), val]
		# model[train_labels[image], :, :] += image_features[image, :, :]
		return model

	def apply_laplace_smoothing(self, model, constant):
		"""
		:param model: (np.array) - (labels, features_per_image)
		:return: smoothed_model: (np.array) - (labels, features_per_image)
		"""
		# for label in np.arange(self.num_labels):
		# 	for feat_num in np.arange(self.total_feats_per_image):
		# 		for val_idx in np.arange(self.values_per_feat):
		# 			model[label, int(feat_num), val_idx] += constant
		# 			self.examples_per_class[label] += (self.values_per_feat * constant)

		for label in np.arange(self.num_labels):
			model[label, : , :] += constant
			self.examples_per_class[label] += (constant * self.values_per_feat * (self.total_feats_per_image * self.values_per_feat))
			model[label, :, :] /= self.examples_per_class[label]
		return model

	def convert_to_value(self, feature):
		'''
		INPUT: feature - np.array(row_feat x col_feat) - flattened
		OUTPUT: value - 0 - 2^(row_feat x col_feat)
		'''
		value = 0
		rev_feature = feature[::-1].tolist()
		value = int("".join(str(int(x)) for x in rev_feature),2)
		return value

	def convert_to_features(self, image_data, disjoint, train_labels):
		"""
		:param image_data:
			   disjoint:
		:return:
		"""
		model = np.zeros((self.num_labels, int(self.total_feats_per_image), self.values_per_feat))
		if disjoint:
			image_features = np.zeros((image_data.shape[0], int(self.total_feats_per_image), self.values_per_feat))
			start_row_idx = np.arange(0,32,self.feat_row_len)
			start_col_idx = np.arange(0,32,self.feat_col_len)
			for image in np.arange(image_data.shape[0]):
				# print(image)
				feature_count = 0
				for row_idx in start_row_idx:
					for col_idx in start_col_idx:
						value = self.convert_to_value(image_data[image, row_idx:row_idx+ self.feat_row_len, col_idx:col_idx + self.feat_col_len].flatten())
						image_features[image, feature_count, value] += 1
						model[train_labels[image],feature_count, value] += 1
						feature_count += 1
		else:
			image_features = np.zeros((image_data.shape[0], int(self.total_feats_per_image), self.values_per_feat))
			start_row_idx = np.arange(self.image_rows - self.feat_row_len + 1)
			start_col_idx = np.arange(self.image_cols - self.feat_col_len + 1)
			for image in np.arange(image_data.shape[0]):
				# print(image)
				feature_count = 0
				for row_idx in start_row_idx:
					for col_idx in start_col_idx:
						value = self.convert_to_value(image_data[image, row_idx:row_idx + self.feat_row_len, col_idx:col_idx + self.feat_col_len].flatten())
						image_features[image, feature_count, value] += 1
						model[int(train_labels[image]),feature_count,value] += 1
						feature_count += 1
		return image_features, model

	def get_features_and_predict(self, test_data, disjoint):
		"""
		:param test_data:
		:param disjoint:
		:return:
		"""
		predicted_labels = np.zeros(test_data.shape[0])
		if disjoint:
			image_features = np.zeros((test_data.shape[0], int(self.total_feats_per_image), self.values_per_feat))
			start_row_idx = np.arange(0, 32, self.feat_row_len)
			start_col_idx = np.arange(0, 32, self.feat_col_len)
			for image in np.arange(test_data.shape[0]):
				# print(image)
				feature_count = 0
				scores = np.zeros((self.num_labels, 1))
				scores[:] += np.log(self.priors)
				for row_idx in start_row_idx:
					for col_idx in start_col_idx:
						value = self.convert_to_value(test_data[image, row_idx:row_idx + self.feat_row_len, col_idx:col_idx + self.feat_col_len].flatten())
						image_features[image, feature_count, value] += 1
						scores[:] += np.reshape(np.log(self.model[:, feature_count, value]), (self.num_labels, 1))
						feature_count += 1
				pr_label = np.argmax(scores)
				predicted_labels[image] = pr_label
		else:
			image_features = np.zeros((test_data.shape[0], int(self.total_feats_per_image), self.values_per_feat))
			start_row_idx = np.arange(self.image_rows - self.feat_row_len + 1)
			start_col_idx = np.arange(self.image_cols - self.feat_col_len + 1)
			for image in np.arange(test_data.shape[0]):
				# print(image)
				feature_count = 0
				scores = np.zeros((self.num_labels, 1))
				scores[:] += np.log(self.priors)
				for row_idx in start_row_idx:
					for col_idx in start_col_idx:
						value = self.convert_to_value(test_data[image, row_idx:row_idx + self.feat_row_len, col_idx:col_idx + self.feat_col_len].flatten())
						image_features[image, feature_count, value] += 1
						scores[:] += np.reshape(np.log(self.model[:, feature_count, value]), (self.num_labels, 1))
						feature_count += 1
				pr_label = np.argmax(scores)
				predicted_labels[image] = pr_label

		return image_features,predicted_labels

	# def predict(self, test_data, disjoint):
	# 	"""
	# 	:param: test_data - np.array - (images, rows, cols)
	# 	:return: predicted_labels - np.array - (images)
	# 	"""
	# 	print("Predicting: ")
	# 	# test_features, extra = self.convert_to_features(test_data, disjoint, np.zeros(test_data.shape[0]))
	# 	test_features, predicted_labels = self.get_features_and_predict(test_data. disjoint)
	# 	predicted_labels = np.zeros(test_data.shape[0])
	# 	for image in np.arange(test_data.shape[0]):
	# 		# print(image)
	# 		scores = np.zeros(self.num_labels)
	# 		scores = np.reshape(scores, (scores.shape[0],1))
	# 		scores[:] += np.log(self.priors)
	# 		for feature in np.arange(self.total_feats_per_image):
	# 			val_idx = np.argmax(test_features[image, int(feature), :])
	# 			scores[:] += np.reshape(np.log(self.model[:, int(feature), val_idx]), (self.num_labels, 1))
	# 		predicted_labels[image] = np.argmax(scores)
	# 	return predicted_labels

	def accuracy(self, test_labels, predicted_labels):
		"""
		:param test_labels:
		:param predicted_labels:
		:return:
		"""
		confusion_matrix = np.zeros((10, 10))
		accuracy_array = np.zeros(10)
		count_array = np.zeros(10)
		total_count = 0
		for i in np.arange(predicted_labels.shape[0]):
			count_array[int(test_labels[i])] += 1
			if (predicted_labels[i] == int(test_labels[i][0])):
				accuracy_array[int(test_labels[i])] += 1
				total_count += 1
			confusion_matrix[int(test_labels[i])][int(predicted_labels[i])] += 1

		print('Total Acc:', total_count/test_labels.shape[0], ' for row_len = ', self.feat_row_len, 'for col_len', self.feat_col_len)
		# for i in np.arange(10):
		# 	print("For digit:", i, " accuracy is:", accuracy_array[i] / count_array[i])

		return confusion_matrix
