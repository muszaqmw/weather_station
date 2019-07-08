from pony import orm
from .base import db


class Device(db.Entity):
    id = orm.PrimaryKey(int, auto=True)
    token = orm.Required(str)
    description = orm.Required(str)
    measures = orm.Set("Measure")