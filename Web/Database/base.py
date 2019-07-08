from pony import orm
from Config import Config

db = orm.Database()
db.bind(provider="mysql",
        host=Config['DB_ADDRESS'],
        user=Config['DB_USER'],
        passwd=Config['DB_PASSWORD'],
        db=Config['DB_NAME'])
