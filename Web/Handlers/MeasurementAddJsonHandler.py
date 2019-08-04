# make request like : http://127.0.0.1:5000/Measurement/AddJson/%7B%22token%22%20:%20%223IF8GRA2UTZD%22,%22measurements%22%20:[%7B%20%22id%22%20:%203,%20%22value%22%20:%2033%7D,%7B%20%22id%22%20:%204,%20%22value%22%20:%2010.5%7D]%7D
# I mean : http://127.0.0.1:5000/Measurement/AddJson/{"token" : "3IF8GRA2UTZD","measurements" :[{ "id" : 3, "value" : 33},{ "id" : 4, "value" : 10.5}]}


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
