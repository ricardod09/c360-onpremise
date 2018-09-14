import sys # used to exit
from kafka import KafkaConsumer

KAFKA_BROKERS = 'localhost:9092'
KAFKA_TOPIC = 'test1'

consumer = KafkaConsumer(KAFKA_TOPIC, bootstrap_servers=KAFKA_BROKERS,
                         auto_offset_reset='earliest')

try:
    for message in consumer:
        print(message.value)
except KeyboardInterrupt:
    sys.exit()
