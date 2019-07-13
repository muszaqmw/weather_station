from pony import orm
from Database import MeasureType
from flask import render_template


@orm.db_session
def handle(request):
    types = orm.select(d for d in MeasureType.MeasureType)

    return render_template('measurementTypes.html', types=types)