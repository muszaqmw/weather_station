from pony import orm
from Database import Device, MeasureType, Measure
import json


@orm.db_session
def handle(request, dataInput):
    returned_info = ""
    if request.method == 'GET':
        data = json.loads(dataInput)

        if not data['token']:
            return "200 : Token not set in arg"

        devices = list(Device.Device.select(lambda d : d.token == data['token']))
        if len(devices) != 1:
            return "201 : Could not found device with that token"
        device = devices[0]

        if len(data['measurements']) == 0:
            return "300 : Measurement list is empty"

        for measurement in data['measurements']:
            type = MeasureType.MeasureType.get(id = measurement['id'])
            if not type:
                returned_info += "300 : Could not add measurement with id %d because it doesn't exists/n" % (measurement['id'])
                continue

            Measure.Measure(device = device, type = type, value = measurement['value'])
            orm.commit()
            returned_info += "100 : Added measure with id %d and value %s \n" % (measurement['id'], measurement['value'])

    return returned_info
