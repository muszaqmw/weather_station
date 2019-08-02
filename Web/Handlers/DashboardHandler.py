from flask import render_template
from pony import orm
from Database import Device,MeasureType


@orm.db_session
def handle(request):
    measures = None
    devices = orm.select(d for d in Device.Device)
    types = orm.select(d for d in MeasureType.MeasureType)

    device_id = request.form.get('device')
    type_id = request.form.get('type')

    if device_id:
        device = Device.Device.get(id=device_id)
        if device:
            measures = device.measures
    else:
        device_id = -1

    if type_id and type_id != "-1":
        measures = measures.filter(lambda m: m.type.id == type_id)
    else:
        type_id = -1

    print(type_id)
    print(device_id)
    print(measures)

    return render_template('dashboard.html', devices=devices, types=types,measures=measures,
                           device_id=int(device_id), type_id=int(type_id))
