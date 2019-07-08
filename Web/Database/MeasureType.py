from pony import orm
from .base import db


class MeasureType(db.Entity):
    id = orm.PrimaryKey(int, auto=True)
    name = orm.Required(str)
    unit = orm.Required(str)
    measures = orm.Set("Measure")