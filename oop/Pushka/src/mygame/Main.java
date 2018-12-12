package mygame;

import com.jme3.app.SimpleApplication;
import com.jme3.asset.TextureKey;
import com.jme3.bullet.BulletAppState;
import com.jme3.bullet.control.RigidBodyControl;
import com.jme3.font.BitmapText;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.material.Material;
import com.jme3.math.Vector2f;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Sphere;
import com.jme3.texture.Texture;

public class Main extends SimpleApplication {

    private BulletAppState bulletAppState;
    private Material stoneMat;
    private Material floorMaterial;

    private RigidBodyControl ballPhysics;
    private Sphere sphere;
    private RigidBodyControl floorPhysics;
    private Box floor;

    public static void main(String[] args) {
        Main main = new Main();
        main.start();
    }

    /**
     * Every time the shoot action is triggered, a new cannon ball is produced.
     * The ball is set up to fly from the camera position in the camera
     * direction.
     */
    private final ActionListener actionListener = new ActionListener() {
        @Override
        public void onAction(String name, boolean keyPressed, float tpf) {
            if (name.equals("shoot") && !keyPressed) {
                makeCannonBall();
            }
        }
    };

    @Override
    public void simpleInitApp() {
        /**
         * Set up Physics Game
         */
        sphere = new Sphere(32, 32, 0.4f, true, false);

        /**
         * Initialize the floor geometry
         */
        floor = new Box(80f, 0.1f, 40f);
        floor.scaleTextureCoordinates(new Vector2f(3, 6));
        bulletAppState = new BulletAppState();
        stateManager.attach(bulletAppState);

        /**
         * Configure cam to look at scene
         */
        cam.setLocation(new Vector3f(0, 4f, 6f));
        cam.lookAt(new Vector3f(2, 2, 0), Vector3f.UNIT_Y);

        /**
         * Initialize the scene, materials, inputs, and physics space
         */
        initHandlers();
        initMaterials();
        initFloor();
        initAim();
    }

    /**
     * Add InputManager action: Left click triggers shooting.
     */
    private void initHandlers() {
        inputManager.addMapping("shoot", new MouseButtonTrigger(MouseInput.BUTTON_LEFT));
        inputManager.addListener(actionListener, "shoot");
    }

    /**
     * Initialize the materials used in this scene.
     */
    public void initMaterials() {
        stoneMat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        TextureKey key1 = new TextureKey("Textures/Terrain/Rock/Rock.PNG");
        Texture texture1 = assetManager.loadTexture(key1);
        stoneMat.setTexture("ColorMap", texture1);

        floorMaterial = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        TextureKey key2 = new TextureKey("Textures/Terrain/Pond/Pond.jpg");
        Texture texture2 = assetManager.loadTexture(key2);
        texture2.setWrap(Texture.WrapMode.Repeat);
        floorMaterial.setTexture("ColorMap", texture2);
    }

    /**
     * Make a solid floor and add it to the scene.
     */
    public void initFloor() {
        Geometry floorGeometry = new Geometry("Floor", floor);
        floorGeometry.setMaterial(floorMaterial);
        floorGeometry.setLocalTranslation(0, -0.1f, 0);
        this.rootNode.attachChild(floorGeometry);

        /* Make the floor physical with mass 0.0f! */
        floorPhysics = new RigidBodyControl(0.0f);
        floorGeometry.addControl(floorPhysics);
        bulletAppState.getPhysicsSpace().add(floorPhysics);
    }

    /**
     * This method creates one individual physical cannon ball. By default, the
     * ball is accelerated and flies from the camera position in the camera
     * direction.
     */
    public void makeCannonBall() {
        /**
         * Create a cannon ball geometry and attach to scene graph.
         */
        Geometry ballGeometry = new Geometry("cannon ball", sphere);
        ballGeometry.setMaterial(stoneMat);
        rootNode.attachChild(ballGeometry);

        /**
         * Position the cannon ball
         */
        ballGeometry.setLocalTranslation(cam.getLocation());
        //ballGeometry.setLocalTranslation(new Vector3f(10, 10, 30));
        float mass = 5;
        float g = 100;

        /**
         * Make the ball physical with a mass > 0.0f
         */
        ballPhysics = new RigidBodyControl(mass);

        /**
         * Add physical ball to physics space.
         */
        ballGeometry.addControl(ballPhysics);
        bulletAppState.getPhysicsSpace().add(ballPhysics);

        /**
         * Accelerate the physical ball to shoot it.
         */
        ballPhysics.setLinearVelocity(cam.getDirection().mult(g / mass));
    }

    /**
     * A plus sign used as crosshairs to help the player with aiming.
     */
    protected void initAim() {
        setDisplayStatView(false);
        BitmapText ch = new BitmapText(guiFont, false);
        ch.setSize(guiFont.getCharSet().getRenderedSize() * 2);
        ch.setText("+");
        ch.setLocalTranslation(settings.getWidth() / 2, settings.getHeight() / 2, 0);
        guiNode.attachChild(ch);
    }
}
