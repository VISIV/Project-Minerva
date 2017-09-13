#gets training data then evaluates sensor data for yield percentage prediction
from keras.models import Sequential
from keras.layers import Dense
import numpy
seed = 7
numpy.random.seed(seed)
# load dataset
dataset = numpy.loadtxt("YieldTraining.csv", delimiter=",")
# split into input (X) and output (Y) variables
X = dataset[:,0:7]
Y = dataset[:,7]
# create model
model = Sequential()
model.add(Dense(12, input_dim=7, init='uniform', activation='relu'))
model.add(Dense(7, init='uniform', activation='relu'))
model.add(Dense(1, init='uniform', activation='sigmoid'))
# Compile model
model.compile(loss='mean_absolute_error', optimizer='adam', metrics=['accuracy'])
# Fit the model
model.fit(X, Y, nb_epoch=150, batch_size=10,  verbose=2)
# calculate predictions
dataset1 = numpy.loadtxt("RawData.csv", delimiter=",")
X = dataset1[:,0:7]
predictions = model.predict(X)
# round predictions
rounded = [round(x[0]) for x in predictions]
numpy.savetxt("YieldDecision.csv", rounded, delimiter=",")
