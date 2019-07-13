from . import LoginHandler
from . import DashboardHandler
from . import DeviceShowHandler
from . import DeviceAddHandler
from . import DeviceRemoveHandler
from . import UserShowHandler
from . import UserLogoutHandler
from . import UserEditHandler
from . import UserRemoveHandler
from . import MeasurementTypeShowHandler
from . import MeasurementTypeAddHandler
from . import MeasurementTypeRemoveHandler


loginHandler = LoginHandler.handle
dashboardHandler = DashboardHandler.handle
deviceShowHandler = DeviceShowHandler.handle
deviceAddHandler = DeviceAddHandler.handle
deviceRemoveHandler = DeviceRemoveHandler.handle
userShowHandler = UserShowHandler.handle
userLogoutHandler = UserLogoutHandler.handle
userEditHandler = UserEditHandler.handle
userRemoveHandler = UserRemoveHandler.handle
measurementTypeShowHandler = MeasurementTypeShowHandler.handle
measurementTypeAddHandler = MeasurementTypeAddHandler.handle
measurementTypeRemoveHandler = MeasurementTypeRemoveHandler.handle
