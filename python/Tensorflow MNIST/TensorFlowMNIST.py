# MNIST For ML Beginners!

# This tutorial is intended for readers who are new to both machine learning and TensorFlow.
# Just like programming has Hello World, machine learning has MNIST.
# MNIST is a simple computer vision dataset. It consists of images of handwritten digits.
# Source: https://goo.gl/B14py7

# Please help us to improve this section by sending us your
# feedbacks and comments on: https://docs.google.com/forms/d/16fH20Qf8gJ2o31Vnlss2uLJ7wL9vq76TeUGqghTY0uI/viewform

# Importing input data
import random
import input_data
mnist = input_data.read_data_sets(raw_input(), raw_input(), raw_input())


import tensorflow as tf

#Input placeholder
x = tf.placeholder(tf.float32, [None, 784])

# Variables to learn
weights = tf.Variable(tf.zeros([784, 10]))
bias = tf.Variable(tf.zeros([10]))

#Our model : softmax
#y is the predicted probability distribution
y = tf.nn.softmax(tf.matmul(x, weights) + bias)

#Training part
#y prime is the true distribution
#cross entropy measures how innacurate our predictions are to predict the truth
yprime = tf.placeholder(tf.float32, [None, 10])
#cross_entropy = tf.nn.softmax_cross_entropy_with_logits(tf.matmul(x, weights) + bias, yprime)
cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(y, yprime))

#back propagation algorithm
#0.5 is the learning rate
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)

init = tf.initialize_all_variables()

#launch the model in a session
sess = tf.InteractiveSession()
sess.run(init)

#train the model chunk of data by chunk of data
for i in range(1000):
    batch_xs, batch_ys = mnist.train.next_batch(100)
    sess.run(train_step, feed_dict={x: batch_xs, yprime: batch_ys})


#print ' '.join(map(str, [random.randint(0,9) for _ in range(len(mnist.validation.images))]))

correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(yprime, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

#print(sess.run(accuracy, feed_dict={x: mnist.test.images, yprime: mnist.test.labels}))
#print(accuracy.eval(feed_dict={x: mnist.test.images, yprime: mnist.test.labels}))

# Uncomment to get a prediction number for each image

result = sess.run(tf.argmax(y,1), feed_dict={x: mnist.validation.images})
print ' '.join(map(str, result))
