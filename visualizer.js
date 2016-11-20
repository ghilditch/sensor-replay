Qt.include("three.js")

var camera, scene, renderer;

// Meshes
var athelete = null;
var bone_helper = null;

// Update type
var updateByPosAndOrientation = false;

// Math helper
var ToRad = Math.PI / 180;

// Main view
var qmlView;

// Materials
var bikerMaterials = [];

function initializeGL(canvas, eventSource, mainView) {

    qmlView = mainView;

    renderer = new THREE.Canvas3DRenderer(
                { canvas: canvas, antialias: true, devicePixelRatio: canvas.devicePixelRatio });
    renderer.setSize(canvas.width, canvas.height);
    renderer.gammaInput = true;
    renderer.gammaOutput = true;
    renderer.shadowMap.enabled = true;
    renderer.shadowMap.renderReverseSided = false;
    renderer.setClearColor(new THREE.Color("cyan"));

    scene = new THREE.Scene();
    scene.fog = new THREE.Fog( 0xffffff, 1, 5000 );
    scene.fog.color.setHSL( 0.6, 0, 1 );

    camera = new THREE.PerspectiveCamera( 60, canvas.width / canvas.height, 1, 1000 );
    //camera.position.set( 0, 4, 0 );
    //camera.lookAt(scene.position);
    moveCamera(-45, 110 , -5);

    var hemiLight = new THREE.HemisphereLight( 0xffffff, 0xffffff, 0.25 );
    hemiLight.color.setHSL( 0.6, 1, 0.6 );
    hemiLight.groundColor.setHSL( 0.095, 1, 0.75 );
    hemiLight.position.set( 0, 500, 0 );
    scene.add( hemiLight );

    var dirLight = new THREE.DirectionalLight( 0xffffff, 1 );
    dirLight.color.setHSL( 0.1, 1, 0.95 );
    dirLight.position.set( -1, 1.75, 1 );
    dirLight.position.multiplyScalar( 50 );
    scene.add( dirLight );

    dirLight.castShadow = true;

    dirLight.shadow.mapSize.width = 2048;
    dirLight.shadow.mapSize.height = 2048;

    var d = 50;

    dirLight.shadow.camera.left = -d;
    dirLight.shadow.camera.right = d;
    dirLight.shadow.camera.top = d;
    dirLight.shadow.camera.bottom = -d;

    dirLight.shadow.camera.far = 3500;
    dirLight.shadow.bias = -0.0001;

    // Create the floor
    createGrid();

    // Load the biker
    loadBikerMesh();
}

var moveCamera = function (h,v,d) {
    var center = {x:0, y:0, z:0};
    camera.position.copy(orbit(center, h, v, d));
    camera.lookAt(center);
}

function orbit(origine, h, v, distance) {
    origine = origine || new THREE.Vector3();
    var p = new THREE.Vector3();
    var phi = v*Math.PI / 180;
    var theta = h*Math.PI / 180;
    p.x = (distance * Math.sin(phi) * Math.cos(theta)) + origine.x;
    p.z = (distance * Math.sin(phi) * Math.sin(theta)) + origine.z;
    p.y = (distance * Math.cos(phi)) + origine.y;
    return p;
}

function createGrid(){
    var size = 14;
    var step = 1;

    var geometry = new THREE.Geometry();
    var material = new THREE.LineBasicMaterial({color: 'white'});

    for (var i = -size; i <= size; i+=step){
        geometry.vertices.push(new THREE.Vector3(-size, -0.04, i));
        geometry.vertices.push(new THREE.Vector3(size, -0.04, i));

        geometry.vertices.push(new THREE.Vector3(i, -0.04, -size));
        geometry.vertices.push(new THREE.Vector3(i ,-0.04, size));
    }

    var lines = new THREE.Line(geometry, material, THREE.LinePieces);
    scene.add (lines);
}

function loadBikerMesh(){
    var request = new XMLHttpRequest();
    request.open("GET", "models/biker.json");
    request.onreadystatechange = function () {
        if (request.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request.responseText), addBikerToScene );
        }
    }
    request.send();
}

function addBikerToScene ( geometry, materials ) {

    // Create our own materials instead of those from Blender
    createMaterials();
    var material = new THREE.MeshFaceMaterial (bikerMaterials);

    athelete = new THREE.SkinnedMesh( geometry, material );
    // Match notch reference rotation
    athelete.rotation.x = 90 * ToRad;
    //athelete.rotation.y = 90 * ToRad;
    //athelete.rotation.z = 90 * ToRad;
    scene.add ( athelete );

    // Add the bone helper
    bone_helper = new THREE.SkeletonHelper(athelete);
    //scene.add (bone_helper);
};

function onRotateCamera(value) {
    moveCamera (value, 110 , -5)
}

function setCameraDistance(distance) {
    //cameraDistance = distance;

}

function onResizeGL(canvas) {
    camera.aspect = canvas.width / canvas.height;
    camera.updateProjectionMatrix();

    renderer.setPixelRatio(canvas.devicePixelRatio);
    renderer.setSize(canvas.width, canvas.height);
}

function paintGL(canvas) {

    // Render the scene
    renderer.render( scene, camera );
}

function isAtheleteLoaded(){
    if( athelete !== null && bone_helper !== null){
        return true;
    }else{
        return false;
    }
}

function updateMovement(bone, x, y, z, q1, q2, q3, q4, angle){

    if (isAtheleteLoaded()){
        if (updateByPosAndOrientation){
            updateMovementByPosAndOrientation (bone, x, y, z, q1, q2, q3, q4);
        }else{
            //console.log("update bone ", bone);
            var b = getBone (bone);
            setBoneAngle (b, angle);
        }
    }
}

function updateMovementByPosAndOrientation(bone, x, y, z, q1, q2, q3, q4){

    var pos = new THREE.Vector3(x, y, z)
    var quat = new THREE.Quaternion(q1, q2, q3, q4);

    var b = getBone (bone);
    if (b === null){
        console.log("Bone not found = ", bone);
    }else{
        //console.log("update bone ", bone);
        b.quaternion.copy(quat);
        b.position.copy(pos);
    }
}

function setBoneAngle(bone, angle){
    var angleInRad = angle / 180 * Math.PI;
    var mtx = new THREE.Matrix4();
    var mtx2 = new THREE.Matrix4();
    var pos = new THREE.Vector3();
    var quat = new THREE.Quaternion();

    mtx.makeTranslation( 1, 1, 1);

    mtx2.makeRotationX(angleInRad);
    mtx.multiply( mtx2 );

    pos.setFromMatrixPosition( mtx );
    quat.setFromRotationMatrix( mtx );

    if (bone !== null){
        bone.rotation.setFromRotationMatrix(mtx);
        //console.log("  angle bone ", angle);
    }
}

function getBone (name){
    for (var i = 0, len = bone_helper.bones.length; i < len; i++) {
        if (bone_helper.bones[i].name === name){
            return bone_helper.bones[i];
        }
    }
    return null;
}

function createMaterials(){
    var alpha = .1;
    var beta = .4;
    var gamma = .1;

    var specularShininess = Math.pow( 2, alpha * 10 );
    var specularColour = new THREE.Color( beta * 0.2, beta * 0.2, beta * 0.2 );

    var shoes = new THREE.Color("black");
    var suit = new THREE.Color(beta * 0.2, beta * 0.2, beta * 0.2);
    var skin = new THREE.Color(0xffcd94);

    var material1 = new THREE.MeshPhongMaterial( {
                color: shoes,
                specular: specularColour,
                reflectivity: beta,
                shininess: specularShininess,
                shading: THREE.SmoothShading,
                envMap: null
            } );
    bikerMaterials.push (material1);


    var material2 = new THREE.MeshPhongMaterial( {
                color: skin,
                specular: specularColour,
                reflectivity: beta,
                shininess: specularShininess,
                shading: THREE.SmoothShading,
                envMap: null
            } );
    bikerMaterials.push (material2);

    var material3 = new THREE.MeshPhongMaterial( {
                color: suit,
                specular: specularColour,
                reflectivity: beta,
                shininess: specularShininess,
                shading: THREE.SmoothShading,
                envMap: null
            } );
    bikerMaterials.push (material3);


    bikerMaterials.forEach (function (mat){
        mat.skinning = true;
    });
}
