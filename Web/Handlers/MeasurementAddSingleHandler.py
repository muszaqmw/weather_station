# make request like : http://127.0.0.1:5000/Measurement/Add?token=14dasdasdasd&type_id=3&value=4


from pony import orm
from Database import Device, MeasureType, Measure


@orm.db_session
def handle(request):
    if request.method == 'GET':
        if not request.args.get("token"):
            return "200 : Token not set in arg"

        devices = list(Device.Device.select(lambda d : d.token == request.args.get("token")))
        if len(devices) != 1:
            return "201 : Could not found device with that token"
        device = devices[0]

        if not request.args.get("type_id"):
            return "300 : Measurement list is empty"

        type = MeasureType.MeasureType.get(id = request.args.get("type_id"))
        if not type:
            return "300 : Could not add measurement with id %d because it doesn't exists/n" % (request.args.get("type_id"))

        if not request.args.get("value"):
            return "301: Values is not set"

        Measure.Measure(device=device, type=type, value=request.args.get("value"))
        orm.commit()
        return "100 : Added measure with id %s and value %s \n" % (request.args.get("type_id"), request.args.get("value"))

    return ""
