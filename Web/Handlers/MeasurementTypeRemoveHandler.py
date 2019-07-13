from pony import orm
from flask import redirect, url_for
from Database import MeasureType


@orm.db_session
def handle(request, measurementTypeId):
    if measurementTypeId and measurementTypeId.isnumeric():
        MeasureType.MeasureType[measurementTypeId].delete()
        orm.commit()

    return redirect(url_for("showMeasurementTypes"))
