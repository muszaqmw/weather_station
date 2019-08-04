from flask import render_template
from pony import orm
from Database import Device, MeasureType, Measure


@orm.db_session
def handle(request):
    measures = None
    devices = orm.select(d for d in Device.Device)
    types = orm.select(d for d in MeasureType.MeasureType)

    device_id = request.args.get('device')
    type_id = request.args.get('type')
    filter = request.args.get('filter')

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

    if filter:
        print(filter)
        measures = measures.filter(lambda m: filter in str(m.value) or filter in str(m.time_reported))
    else:
        filter = ""

    if measures:
        measures = measures.order_by(Measure.Measure.time_reported)

    return render_template('dashboard.html', devices=devices, types=types,measures=measures,
                           device_id=int(device_id), type_id=int(type_id), filter=filter)
