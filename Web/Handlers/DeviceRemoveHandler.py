from pony import orm
from flask import redirect, url_for
from Database import Device


@orm.db_session
def handle(request, deviceId):
    print('dev', deviceId)
    if deviceId and deviceId.isnumeric():
        Device.Device[deviceId].delete()
        orm.commit()

    return redirect(url_for("showDevices"))
