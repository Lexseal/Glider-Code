import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
import math

mnist = input_data.read_data_sets("/tmp/data/", one_hot=True)

classCount = 10
batchSize = 128

x = tf.placeholder("float", [None, 28*28])
y = tf.placeholder("float")
learningRate = tf.placeholder("float")

def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides = [1, 1, 1, 1], padding = "SAME")

def maxPool(x):
    return tf.nn.max_pool(x, ksize = [1, 2, 2, 1], strides = [1, 2, 2, 1], padding = "SAME")

def neuralNetModel(data):
    data = tf.reshape(data, shape = [-1, 28, 28, 1])
    intputWeight = {"weights":tf.Variable(tf.random_normal([5, 5, 1, 8])), 
    "biases":tf.Variable(tf.random_normal([8]))}
    weight1 = {"weights":tf.Variable(tf.random_normal([3, 3, 8, 16])), 
    "biases":tf.Variable(tf.random_normal([16]))}
    weight2 = {"weights":tf.Variable(tf.random_normal([2, 2, 16, 32])), 
    "biases":tf.Variable(tf.random_normal([32]))}
    weight3 = {"weights":tf.Variable(tf.random_normal([7*7*32, 512])), 
    "biases":tf.Variable(tf.random_normal([512]))}
    outputWeight = {"weights":tf.Variable(tf.random_normal([512, classCount])), 
    "biases":tf.Variable(tf.random_normal([classCount]))}

    intputLayer = conv2d(data, intputWeight["weights"] + intputWeight["biases"])

    hidLayer1 = conv2d(intputLayer, weight1["weights"]) + weight1["biases"]
    hidLayer1 = maxPool(hidLayer1)

    hidLayer2 = conv2d(hidLayer1, weight2["weights"]) + weight2["biases"]
    hidLayer2 = maxPool(hidLayer2)
    hidLayer2 = tf.reshape(hidLayer2, shape = [-1, 7*7*32]);
    tf.nn.dropout(hidLayer2, 0.75)

    hidLayer3 = tf.matmul(hidLayer2, weight3["weights"]) + weight3["biases"]
    hidLayer3 = tf.nn.relu(hidLayer3)

    outputLayer = tf.matmul(hidLayer3, outputWeight["weights"]) + outputWeight["biases"]

    return outputLayer

def trainNet(x):
    prediction = neuralNetModel(x)
    cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits = prediction, labels = y))
    optimizer = tf.train.AdamOptimizer(learning_rate = learningRate).minimize(cost)

    epochs = 10

    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        
        i = 0
        for epoch in range(epochs):
            epoch_loss = 0
            for _ in range(int(mnist.train.num_examples/batchSize)):
                epochX, epochY = mnist.train.next_batch(batchSize)
                _, c = sess.run([optimizer, cost], feed_dict = {x: epochX, y: epochY, learningRate: math.exp(-i-5.3)+0.0001})
                epoch_loss += c
            i+=1
            print("Epoch", epoch+1, "completed out of ", epochs, " loss: ", epoch_loss)
            correct = tf.equal(tf.argmax(prediction, 1), tf.argmax(y, 1))
            accuracy = tf.reduce_mean(tf.cast(correct, "float"))
            print("Accuracy: ", accuracy.eval({x:mnist.test.images, y:mnist.test.labels}))

trainNet(x)
