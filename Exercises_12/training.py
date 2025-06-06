import numpy as np
import pickle
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.optimizers import SGD, Adam, RMSprop, Adagrad
from tensorflow import keras

seed = 0
np.random.seed(seed)

import tensorflow as tf
import os
tf.random.set_seed(seed)

if not os.path.exists("data"):
    os.makedirs("data")

# Carica dati
img_rows, img_cols = 28, 28
num_classes = 10
(X_train, Y_train), (X_test, Y_test) = mnist.load_data()
X_train = X_train.reshape(X_train.shape[0], img_rows*img_cols).astype('float32') / 255
X_test = X_test.reshape(X_test.shape[0], img_rows*img_cols).astype('float32') / 255
Y_train = keras.utils.to_categorical(Y_train, num_classes)
Y_test = keras.utils.to_categorical(Y_test, num_classes)

def create_DNN():
    model = Sequential()
    model.add(Dense(400, input_shape=(img_rows*img_cols,), activation='relu'))
    model.add(Dense(100, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(num_classes, activation='softmax'))
    return model

def compile_model(optimizer):
    model = create_DNN()
    model.compile(loss=keras.losses.categorical_crossentropy,
                  optimizer=optimizer,
                  metrics=['acc'])
    return model

optimizer_dict = {
    'SGD': SGD(),
    'Adam': Adam(),
    'RMSprop': RMSprop(),
    'Adagrad': Adagrad()
}

epochs = 50
batch_size = 32
histories = {}
scores = {}

for opt_name, opt in optimizer_dict.items():
    print('Training with optimizer:', opt_name)
    model_DNN = compile_model(optimizer=opt)
    history = model_DNN.fit(
        X_train, Y_train,
        batch_size=batch_size,
        epochs=epochs,
        shuffle=True,
        verbose=2,
        validation_data=(X_test, Y_test)
    )
    histories[opt_name] = history.history
    score = model_DNN.evaluate(X_test, Y_test, verbose=0)
    scores[opt_name] = score
    print(f'Test loss with {opt_name}:', score[0])
    print(f'Test accuracy with {opt_name}:', score[1])

with open("data/dnn_histories.pkl", "wb") as f:
    pickle.dump(histories, f)
with open("data/dnn_scores.npy", "wb") as f:
    np.save(f, scores)