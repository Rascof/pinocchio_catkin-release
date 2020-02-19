import unittest
import pinocchio as pin
pin.switchToNumpyMatrix()
import numpy as np
from pinocchio.utils import zero,rand

class TestForceBindings(unittest.TestCase):

    def test_zero_getters(self):
        f = pin.Force.Zero()
        self.assertTrue(np.allclose(zero(3), f.linear))
        self.assertTrue(np.allclose(zero(3), f.angular))
        self.assertTrue(np.allclose(zero(6), f.vector))

    # TODO: this is not nice, since a random vector can *in theory* be zero
    def test_setRandom(self):
        f = pin.Force.Zero()
        f.setRandom()
        self.assertFalse(np.allclose(zero(3), f.linear))
        self.assertFalse(np.allclose(zero(3), f.angular))
        self.assertFalse(np.allclose(zero(6), f.vector))

    def test_setZero(self):
        f = pin.Force.Zero()
        f.setRandom()
        f.setZero()
        self.assertTrue(np.allclose(zero(3), f.linear))
        self.assertTrue(np.allclose(zero(3), f.angular))
        self.assertTrue(np.allclose(zero(6), f.vector))

    def test_set_linear(self):
        f = pin.Force.Zero()
        lin =  rand(3)
        f.linear = lin
        self.assertTrue(np.allclose(f.linear, lin))

    def test_set_angular(self):
        f = pin.Force.Zero()
        ang = rand(3)
        f.angular = ang
        self.assertTrue(np.allclose(f.angular, ang))

    def test_set_vector(self):
        f = pin.Force.Zero()
        vec =  rand(6)
        f.vector = vec
        self.assertTrue(np.allclose(f.vector, vec))

    def test_internal_sums(self):
        f1 = pin.Force.Random()
        f2 = pin.Force.Random()
        self.assertTrue(np.allclose((f1+f2).vector,f1.vector + f2.vector))
        self.assertTrue(np.allclose((f1 - f2).vector, f1.vector - f2.vector))

    def test_se3_action(self):
        f = pin.Force.Random()
        m = pin.SE3.Random()
        self.assertTrue(np.allclose((m * f).vector,  np.linalg.inv(m.action.T) * f.vector))
        self.assertTrue(np.allclose(m.act(f).vector, np.linalg.inv(m.action.T) * f.vector))
        self.assertTrue(np.allclose((m.actInv(f)).vector, m.action.T * f.vector))
        v = pin.Motion(np.vstack([f.vector[3:], f.vector[:3]]))
        self.assertTrue(np.allclose((v ^ f).vector, zero(6)))

    def test_conversion(self):

        f = pin.Force.Random()
        f_array = np.array(f)

        f_from_array = pin.Force(f_array)

        self.assertTrue(f_from_array == f)

if __name__ == '__main__':
    unittest.main()
