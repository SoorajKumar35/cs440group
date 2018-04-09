import support as sup 
from bayesian_model import bayesian_model
from bayesian_model_v2 import bayesian_model_v2
import numpy as np
if __name__ == '__main__':

	testing = 1
	# Testing for 1.1
	train_data, train_labels = sup.read_data('digitdata/optdigits-orig_train.txt')
	test_data, test_labels = sup.read_data('digitdata/optdigits-orig_test.txt')

	if(testing == 1):

		constants = [0.1, 0.5, 1, 2.5, 5, 7.5, 10, 15, 20]
		for constant in constants:
			print(constant)
			model = bayesian_model(train_data, train_labels, constant)

			predicted_labels = model.predict(test_data,test_labels)

			confusion_matrix = model.accuracy(predicted_labels, test_labels)

			# model.odds_ratios(confusion_matrix)

	# Testing for 1.2
	if(testing == 2):

		constants = [0.1,0.5,1,2.5,5,7.5,10,15,20]
		for constant in constants:

			disjoint = True
			x = [2,4]
			y = [2,4]
			for x_val in x:
				for y_val in y:
					model_v2 = bayesian_model_v2(train_data, train_labels, x_val, y_val, constant, disjoint)

					test_features, predicted_labels = model_v2.get_features_and_predict(test_data, disjoint)

					confusion_matrix = model_v2.accuracy(test_labels, predicted_labels)

			print("Now for overlaps\n")

			x2 = [2,3,4]
			y2 = [2,3,4]
			disjoint = False
			for x_val in x2:
				for y_val in y2:
					if (x_val == 4 and y_val == 3) or (x_val == 3 and y_val == 4):
						continue
					model_v2 = bayesian_model_v2(train_data, train_labels, x_val, y_val, constant, disjoint)

					test_features, predicted_labels = model_v2.get_features_and_predict(test_data, disjoint)

					confusion_matrix = model_v2.accuracy(test_labels, predicted_labels)

			print('\n')

	if(testing == 3):
		face_train_data, face_train_labels = sup.read_face_data('facedata/facedatatrain', 'facedata/facedatatrainlabels')
		face_test_data, face_test_labels = sup.read_face_data('facedata/facedatatest', 'facedata/facedatatestlabels')

		model = bayesian_model(face_train_data, np.reshape(face_train_labels, (face_train_labels.shape[0],1)), 0.1)

		predicted_labels = model.predict(face_test_data, np.reshape(face_test_labels, (face_test_labels.shape[0],1)))

		confusion_matrix = model.accuracy(predicted_labels, np.reshape(face_test_labels, (face_test_labels.shape[0],1)))












