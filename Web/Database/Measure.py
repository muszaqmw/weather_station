from pony import orm
from .Device import Device
from .MeasureType import MeasureType
from .base import db
from datetime import datetime


class Measure(db.Entity):
    id = orm.PrimaryKey(int, auto=True)
    device = orm.Required(Device)
    type = orm.Required(MeasureType)
    value = orm.Required(float)
    time_reported = orm.Required(datetime, sql_default='CURRENT_TIMESTAMP')