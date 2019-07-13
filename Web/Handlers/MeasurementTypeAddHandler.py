from pony import orm
from flask import redirect, url_for
from Database import MeasureType

@orm.db_session
def handle(request):
    if request.method == 'POST':
        name = request.form['name']
        unit = request.form['unit']

        if name and unit:
            MeasureType.MeasureType(name=name, unit=unit)
            orm.commit()

    return redirect(url_for('showMeasurementTypes'))