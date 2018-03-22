import support as sup 
from bayesian_model import bayesian_model

if __name__ == '__main__':
	
	train_data,train_labels = sup.read_data('digitdata/optdigits-orig_train.txt')

	model = bayesian_model(train_data, train_labels)

	test_data, test_labels = sup.read_data('digitdata/optdigits-orig_test.txt')

	predicted_labels = model.predict(test_data)

	model.accuracy(predicted_labels, test_labels)










