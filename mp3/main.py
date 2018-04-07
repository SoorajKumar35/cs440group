import support as sup 
from bayesian_model import bayesian_model
from bayesian_model_v2 import bayesian_model_v2

if __name__ == '__main__':

	testing = 1
	# Testing for 1.1
	train_data, train_labels = sup.read_data('digitdata/optdigits-orig_train.txt')
	test_data, test_labels = sup.read_data('digitdata/optdigits-orig_test.txt')

	if(testing == 1):

		model = bayesian_model(train_data, train_labels)

		predicted_labels = model.predict(test_data,test_labels)

		confusion_matrix = model.accuracy(predicted_labels, test_labels)

		model.odds_ratios(confusion_matrix)

	# Testing for 1.2
	if(testing == 2):
		model_v2 = bayesian_model_v2(train_data, train_labels, 3, 3, False)

		predicted_labels = model_v2.predict(test_data, False)

		confusion_matrix = model_v2.accuracy(test_labels, predicted_labels)















