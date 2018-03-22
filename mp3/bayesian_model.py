import support
import numpy as np


class bayesian_model:

	def __init__(self, train_data, train_labels):

		self.num_images = train_data.shape[0]
		self.num_labels = 10
		self.priors = self.get_priors(train_labels)
		self.model = self.train_model(train_data,train_labels)


	def train_model(self, train_data, train_labels):
		'''
		INPUT: train_data: (np.array) - (images, rows, cols)
			   train_labels: (np.array) - (labels per image)
		OUTPUT: model: (np.array) - (labels, row, cols)
		'''

		print("Training the model.... ")

		constant = 0.1

		model = np.zeros((self.num_labels, 32, 32))
		
		num_per_class = self.priors * self.num_images

		for image in np.arange(self.num_images):
			for row in np.arange(32):
				for col in np.arange(32):
					model[train_labels[image], row, col] += train_data[image, row, col]

		smoothed_model = self.apply_laplace_smoothing(model, constant)

		for label in np.arange(self.num_labels):
			for row in np.arange(32):
				for col in np.arange(32):
					if(smoothed_model[label, row, col] == 0):
						print("Still got a zero")

		for label in np.arange(self.num_labels):
			smoothed_model[label,:,:] = (smoothed_model[label,:,:]/ (num_per_class[label] + constant*2))

		# for label in np.arange(self.num_labels):
		# 	for row in np.arange(32):
		# 		for col in np.arange(32):
		# 			print(smoothed_model[label,row,col])

		print("Done!")
		return smoothed_model


	def get_priors(self, train_labels):
		'''
		INPUT: train_labels: np.array - (num_samples, 1) - The labels of the training set each corresponding to one observation
		OUTPUT: priors: np.array - (num_labels, 1) - The priors for each class
		'''
		priors = np.zeros((self.num_labels, 1))
		for i in np.arange(self.num_images):
			priors[train_labels[i]] += 1
		priors = priors*(1/self.num_images)
		return priors
	
	def apply_laplace_smoothing(self, model, constant):
		'''
		INPUT: model - (np.array) - (labels, row, cols) - model without laplace smoothing applied
			   constant - float - the small k to add
		OUTPUT: smoothed_model - (np.array) - (labels, row, cols) - model with laplace smoothing applied
		'''

		print("Applying laplace smoothing.... ")

		for label in np.arange(self.num_labels):
			for row in np.arange(32):
				for col in np.arange(32):
					model[label, row, col] += constant
		return model


	def predict(self, test_data):
		'''
		INPUT: test_data - (np.array) - (images, rows, cols) - the testing images
		OUTPUT: predicted_labels - (np.array) - (images,1) - predicted labels for each image
		'''

		predicted_labels = np.zeros((test_data.shape[0],1))
		for image in np.arange(test_data.shape[0]):
			scores = np.zeros((self.num_labels,1))
			for label in np.arange(self.num_labels):
				scores[label][0] += np.log(self.priors[label])
				for row in np.arange(32):
					for col in np.arange(32):
						if(test_data[image,row,col] == 1):
							scores[label][0] += np.log(self.model[label,row,col])
						elif(test_data[image,row,col] == 0):
							scores[label][0] += np.log(1 - self.model[label,row,col])
						else:
							print("Huh? :-O Shunu hada ya habiti?")
			label = np.argmax(scores)
			# print(label)
			predicted_labels[image][0] = label

		return predicted_labels


	def accuracy(self, predicted_labels, test_labels):
		'''
		INPUT: predicted_labels: - (np.array) - (images,1) - predicted labels for each image
		       test_labels - (np.array) - (images,1) - true labels for each image
		OUTPUT: accuracy: - the number of labels predicted correctly
		'''
		print("predicted_labels",predicted_labels.shape[0])
		print("test_labels", test_labels.shape[0])
		num_correct = 0
		for i in np.arange(predicted_labels.shape[0]):
			if(predicted_labels[i][0] == test_labels[i][0]):
				num_correct += 1
		print("Accuracy: ", num_correct/predicted_labels.shape[0])



