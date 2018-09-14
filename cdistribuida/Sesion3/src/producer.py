import os

from kafka import KafkaProducer

KAFKA_BROKERS = 'localhost:9092'
KAFKA_TOPIC = 'test1'

producer = KafkaProducer(bootstrap_servers=KAFKA_BROKERS)

# Must send bytes
messages = [b'hola', b'enviamos', b'un mensaje desde Python']

# Send the messages
for m in messages:
    producer.send(KAFKA_TOPIC, m)
producer.flush()
