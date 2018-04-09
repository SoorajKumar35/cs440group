import support
import numpy as np
import matplotlib.pyplot as plt


class bayesian_model:

	def __init__(self, train_data, train_labels, constant):

		self.constant = constant
		self.rows = train_data.shape[1]
		self.cols = train_data.shape[2]
		self.num_images = train_labels.shape[0]
		self.num_labels = np.unique(train_labels).shape[0]
		self.priors = self.get_priors(train_labels)
		self.model = self.train_model(train_data, train_labels)

	def train_model(self, train_data, train_labels):
		'''
		INPUT: train_data: (np.array) - (images, rows, cols)
			   train_labels: (np.array) - (labels per image)
		OUTPUT: model: (np.array) - (labels, row, cols)
		'''

		print("Training the model.... ")

		constant = self.constant

		model = np.zeros((self.num_labels, self.rows, self.cols))  # P(F_ij = 1| every class)

		num_per_class = self.priors * self.num_images

		for image in np.arange(self.num_images):
			model[train_labels[image][0], :, :] += train_data[image, :, :]

		smoothed_model = self.apply_laplace_smoothing(model, constant, num_per_class)

		for label in np.arange(self.num_labels):
			smoothed_model[label, :, :] = smoothed_model[label, :, :] / (num_per_class[label][0])

		print("Done!")
		return smoothed_model

	def get_priors(self, train_labels):
		'''
		INPUT: train_labels: np.array - (num_samples, 1) - The labels of the training set each corresponding to one observation
		OUTPUT: priors: np.array - (num_labels, 1) - The priors for each class
		'''
		priors = np.zeros((self.num_labels, 1))
		for i in np.arange(self.num_images):
			priors[train_labels[i][0]] += 1
		priors = priors * (1 / self.num_images)
		return priors

	def apply_laplace_smoothing(self, model, constant, num_per_class):
		'''
		INPUT: model - (np.array) - (labels, row, cols) - model without laplace smoothing applied
			   constant - float - the small k to add
			   num_per_class - np.array - (labels, 1) - number of training token per class
		OUTPUT: smoothed_model - (np.array) - (labels, row, cols) - model with laplace smoothing applied
		'''

		print("Applying laplace smoothing.... ")

		for label in np.arange(self.num_labels):
			for row in np.arange(32):
				for col in np.arange(32):
					model[label, row, col] += constant
					num_per_class[label][0] += (constant * 2)
		return model

	def predict(self, test_data ,test_labels):
		'''
		INPUT: test_data - (np.array) - (images, rows, cols) - the testing images
		OUTPUT: predicted_labels - (np.array) - (images,1) - predicted labels for each image
		'''
		print("I am here right now")
		predicted_labels = np.zeros((test_labels.shape[0], 1))
		self.score_per_image = np.zeros(test_labels.shape[0])
		for image in np.arange(test_labels.shape[0]):
			scores = np.zeros((self.num_labels, 1))
			for label in np.arange(self.num_labels):
				scores[label][0] += np.log(self.priors[label][0])
				for row in np.arange(self.rows):
					for col in np.arange(self.cols):
						if (test_data[image, row, col] == 1):
							scores[label][0] += np.log(self.model[label, row, col])
						elif (test_data[image, row, col] == 0):
							scores[label][0] += np.log(1 - self.model[label, row, col])
						else:
							print("Huh? :-O Shunu hada ya habibti?")
			p_label = np.argmax(scores)
			self.score_per_image[image] = scores[test_labels[image]]
			predicted_labels[image][0] = p_label
		print("I have left this section")
		return predicted_labels

	def accuracy(self, predicted_labels, test_labels):
		'''
		INPUT: predicted_labels: - (np.array) - (images,1) - predicted labels for each image
			   test_labels - (np.array) - (images,1) - true labels for each image
		OUTPUT: accuracy: - the accuracy for each digit
				confusion_matrix - a 10x10 matrix that contain which labels were confused for another by the model
		'''

		# for label in np.arange(self.num_labels):
		# 	curr_idxs = np.where(test_labels == label)[0]
		# 	curr_scores = self.score_per_image[curr_idxs]
		# 	sorted_scores_idxs = np.argsort(curr_scores)
		# 	idx_of_zero = np.where(sorted_scores_idxs == 0)[0]
		# 	idx_of_last = np.where(sorted_scores_idxs == sorted_scores_idxs.shape[0]-1)[0]
		# 	print("For label",label,"the lowest prob is:", curr_idxs[idx_of_zero],"while the highest is:",curr_idxs[idx_of_last])

		print('\n')
		confusion_matrix = np.zeros((self.num_labels, self.num_labels))
		accuracy_array = np.zeros(self.num_labels)
		count_array = np.zeros(self.num_labels)
		total_correct = 0
		for i in np.arange(predicted_labels.shape[0]):
			count_array[test_labels[i][0]] += 1
			if (predicted_labels[i][0] == test_labels[i][0]):
				accuracy_array[test_labels[i][0]] += 1
				total_correct += 1
			confusion_matrix[test_labels[i][0]][int(predicted_labels[i][0])] += 1

		print('Total Accuracy:', total_correct/test_labels.shape[0])
		print('\n')
		# for i in np.arange(self.num_labels):
		# 	print("For digit:", i, " accuracy is:", accuracy_array[i] / count_array[i])
        #
		# for row in np.arange(self.num_labels):
		# 	for col in np.arange(self.num_labels):
		# 		confusion_matrix[row][col] /= (count_array[row]/100)
		# 		print(int(confusion_matrix[row][col]), end = '')
		# 		print(' ', end = '')
		# 	print('\n')

		return confusion_matrix

	def odds_ratios(self, confusion_matrix):
		"""
		INPUT: confusion_matrix - (10x10 np.array) - the percentage of class r misclassified as class in c in the test set
		OUTPUT: odds_ratios for each pair of the four classes with the highest percentages in the confusion matrix
				heat maps of the log-likelihood for each of the aformentioned pairs
		"""

		error_rates = np.zeros(self.num_labels)
		for row in np.arange(self.num_labels):
			error_rates[row] += (np.sum(confusion_matrix[row, :]) - confusion_matrix[row][row])

		sorted_error_labels = np.argsort(error_rates)
		top_four = sorted_error_labels[6:]

		odds_ratios = np.zeros((12, self.rows, self.cols))

		pairs_top_four = []
		for pair_1 in np.arange(4):
			for pair_2 in np.arange(4):
				if (pair_1 != pair_2):
					pairs_top_four.append([pair_1, pair_2])

		for pair in range(len(pairs_top_four)):
			first_num = pairs_top_four[pair][0]
			second_num = pairs_top_four[pair][1]
			odds_ratios[pair, :, :] = np.log(self.model[first_num, :, :]) - np.log(self.model[second_num, :, :])


		figure_count = 1
		for pair in range(len(pairs_top_four)):
			subplot_count = 311
			first_num = pairs_top_four[pair][0]
			second_num = pairs_top_four[pair][1]

			plt.figure(figure_count)

			plt.subplot(subplot_count)
			plt.imshow(np.log(self.model[first_num,:,:]))

			subplot_count += 1

			plt.subplot(subplot_count)
			plt.imshow(np.log(self.model[second_num,:,:]))

			subplot_count += 1

			plt.subplot(subplot_count)
			plt.imshow(odds_ratios[pair, :, :])

			plt.show(block=False)

			figure_count += 1

		print("Stop right there sir")

