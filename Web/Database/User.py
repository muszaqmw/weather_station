from pony import orm
from .base import db


class User(db.Entity):
    id = orm.PrimaryKey(int, auto=True)
    name = orm.Required(str)
    password = orm.Required(str)
    token = orm.Required(int)
    ip = orm.Required(str)