using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
//using UnityEditor;

public class TerrainGeneration : MonoBehaviour
{
    public int noiseWidth = 513;
    public int noiseHeight = 513;
    private int mapDepth = 200;

    public bool snowExists = false;
    public float snowMinVal;

    public int saves = 0;

    //set constraints for this!!!!!!!!!!!!!!!!!!!!!!!
    public bool waterExists = false;
    public float waterMaxVal;

    //set constraints for this!!!!!!!!!!!!!!!!!!!!!!!
    public bool treesExist = false;
    public float treesMaxVal;
    public float treesMinVal;

    public int amountOfTrees;
    public int amountOfSnow;

    //user changes
    public float startAmplitude = 1.73f;
    public int octaveCounter = 6;
    public float persistence = 0.7f;

    public Plane waterPlane;

    //save variables for reset etc.
    Terrain terrain;
    public Terrain defaultTerrain;
    public float[,] generatedBaseNoise;
    public Vector3 defaultWaterPos;
    public float defaultAmp;
    public float defaultPer;
    public int defaultOct;

    //UI VARIABLES AND OBJECTS!!
    public Button resetButton;
    public Button saveButton;
    public Button generateNewRandomTerrainButton;
    public Slider amplitudeSlider;
    public Slider persistenceSlider;
    public Slider amountOfTreesSlider;
    public Slider octaveSlider;
    public Button trees;
    public Button water;
    public Button snow;
    public Button moveTerrainLeft;
    public Button moveTerrainRight;
    public Button moveTerrainUp;
    public Button moveTerrainDown;

    //pop up stuff for tree/water intervalls
    public Image wrongInputImage;
    public Text invalidInputText;
    public Text invalidInputText2;
    public Button closePopUpImageButton;

    //pop up stuff for tree/water intervalls
    public Image wrongInputImageSnow;
    public Text invalidInputTextSnow;
    public Text invalidInputText2Snow;
    public Button closePopUpImageButtonSnow;

    public Text TreeText;
    public Text SnowText;
    public Text WaterText;

    public Transform waterTransform;
    public GameObject[] treeArr;
    private bool arrayFilled = false;
    public GameObject[] snowArr;
    private bool snowArrFilled = false;
    public GameObject parentOfTerrain;

    void Start ()
    {
        //generate random noise in start to later transform to controlled smoothnoise 
        //random noise is generated here because the GenerateTerrain is going to be able to run in update without changing the random basenoise all the time
        generatedBaseNoise = GenerateNoise(noiseWidth, noiseHeight);

        terrain = GetComponent<Terrain>();
        defaultTerrain = terrain;                                                                                               //save the default-terrain to later be able to reset the map
        GenerateTerrain(generatedBaseNoise);

        //save the position of water plane for later use of disabling it
        //hide the plane by setting the waterMaxVal aswell
        defaultWaterPos = waterTransform.localPosition;

        //set default values
        treesMaxVal = 50;
        treesMinVal = 20;
        snowMinVal = 150;
        amountOfTrees = 20;
        waterMaxVal = 20;
        amountOfSnow = 20;

        startAmplitude = Random.Range(0.0f,25.0f);
        octaveCounter = (int)Random.Range(0.0f, 9.0f);
        persistence = Random.Range(0.0f, 1.0f);

        defaultAmp = startAmplitude;
        defaultPer = persistence;
        defaultOct = octaveCounter;

    //UI STUFF
    resetButton = GetComponent<Button>();
        saveButton = GetComponent<Button>();
        generateNewRandomTerrainButton = GetComponent<Button>();

        //set the slider handle to the value what the startAmplitude and persistence is currently
        amplitudeSlider.value = startAmplitude;
        persistenceSlider.value = persistence;
        octaveSlider.value = octaveCounter;
        amountOfTreesSlider.value = amountOfTrees;

        //hide the element texts
        TreeText.text = "";
        SnowText.text = "";
        WaterText.text = "";

        //hide panel
        wrongInputImage.enabled = false;
        invalidInputText.enabled = false;
        invalidInputText2.enabled = false;
        closePopUpImageButton.gameObject.SetActive(false);

        //hide panel
        wrongInputImageSnow.enabled = false;
        invalidInputTextSnow.enabled = false;
        invalidInputText2Snow.enabled = false;
        closePopUpImageButtonSnow.gameObject.SetActive(false);

        //set all bools
        snowExists = false;
        waterExists = false;
        treesExist = false;

        //instantiate tree-array
        treeArr = new GameObject[amountOfTrees];
        treeArr = GameObject.FindGameObjectsWithTag("Tree");
        arrayFilled = true;

        //instantiate snow-array
        snowArr = new GameObject[amountOfSnow];
        snowArr = GameObject.FindGameObjectsWithTag("Snow");
        snowArrFilled = true;
    }
	
	// Update is called once per frame
	void Update ()
    {
        GenerateTheTerrain(terrain, generatedBaseNoise);
    }

    void GenerateTerrain(float[,] baseNoise)
    {
        //float[,] baseNoise;

        //set the dimensions of the terrain with the users variables
        SetDimensionsOfMap(noiseWidth, noiseHeight, mapDepth);
        SetTerrainSize(terrain);

        //create the noise for the map and generate in graphically using the terraindata
        terrain.terrainData.SetHeights(0, 0, GenerateFinalNoise(baseNoise));
    }

    //updates the same map that is made in the beginning to show new changes but not ranomize a completely new map
    public void GenerateTheTerrain(Terrain ter, float[,] baseNoise)
    {
        terrain = ter;
        terrain.terrainData.SetHeights(0, 0, GenerateFinalNoise(baseNoise));
    }

    void SetDimensionsOfMap(int width, int height, int depth)
    {
        noiseWidth = width;
        noiseHeight = height;
        mapDepth = depth;
    }

    void SetTerrainSize(Terrain ter)
    {
        ter.terrainData.size = new Vector3(noiseWidth, mapDepth, noiseHeight);
    }

    //generate a random basenoise to later smoothen depending on octave
    //returns a 2d float array with random floats between 0-1
    float[,] GenerateNoise(int width, int height)
    {
        float[,] noise = new float[noiseWidth, noiseHeight];

        for (int x = 0; x < noiseWidth; x++)
        {
            for (int y = 0; y < noiseHeight; y++)
            {
                noise[x, y] = Random.Range(0.0f, 1.0f);
            }
        }
        return noise;
    }

    //generates the final noise 
    //takes a random basenoise and octave(i.e amount of Octaves) and returns a 2d float array with the finished perlin noise for the map
    float[,] GenerateFinalNoise(float[,] baseNoise)
    {
        //all the variables to save the perlin noise, interpolate all smooth noises via amplitude etc.
        float[,] finalNoise = new float[noiseWidth, noiseHeight];
        float amplitude = startAmplitude;
        float totAmpl = 0.0f;
        float[][,] smoothNoises = new float[octaveCounter][,];

        //generate smooth noise from the random basenoise (depending on octave)
        for (int k = 0; k < octaveCounter; k++)
        {
            smoothNoises[k] = GenerateSmoothNoise(baseNoise);
        }

        //blend all noises together by changing the amplitude for all octaves via multiplying it with persistance.

        for (int i = octaveCounter -1; i > 0; i--)
        {
            amplitude = amplitude * persistence;
            totAmpl = totAmpl + amplitude;

            //set the final noise in the same way as in the GenerateNoise function but this time with the smoothed noise and not random numbers
            for (int w = 0; w < noiseWidth; w++)
            {
                for (int h = 0; h < noiseHeight; h++)
                {
                    finalNoise[w, h] = smoothNoises[i][w, h] * amplitude;
                }
            }
        }

        //to get the average of all noises the final noise must be divided with the total amplitude
        
        return finalNoise;
    }

    float[,] GenerateSmoothNoise(float[,] baseNoise)
    {
        float[,] smoothNoise = new float[noiseWidth, noiseHeight];
        int waveLength = (int)Mathf.Pow(2, octaveCounter);
        float frequency = (1.0f / waveLength);

        //loop through x and y in the matrix/array/quad (whichever is better to visualize to understand) to set the horizontal and veritcal sampling indices 
        //as well as the alpha values for the interpolation of all points in the quad/arr/matrix
        for(int x=0; x<noiseWidth; x++)
        {
            //to "zoom" in on the noise and use one small part we make sure that (depending on the octavenumber) every 5, 8 etc index is a new value
            //ex. i=0, value=4; i=1,value=4; i=2,value=4; i=3,value=4; i=4,value=4; i=5,value=8; i=6,value=8; i=7,value=8; etc..
            //this way an array of ex [128][128] is zoomed in so it works with for example [16][16] but is still handled as a [128][128] array by giving every 8th index a new value

            //this is later "wrapped" so that all values can be handled and let the program be fooled that after the end of the list (ex 128) comes 0
            //this way every number will be "transformed" to fit in this process (this only happens att the end of the row of the matrix/arr/quad to later go on with 0 in x-value and then 
            //+1 in y-value. To achieve this the modulus operator is used (to get the rest). If there is no rest we have not come to the end of the row (or in next loop for y, the next col)
            int x0 = (x / waveLength) * waveLength;
            int x1 = (x0 + waveLength) % noiseWidth;
            float horAlpha = (x - x0) * frequency;                                                                                  //get an alpha value that decides the blend (float from 0-1)

            for(int y=0; y<noiseHeight; y++)
            {
                int y0 = (y / waveLength) * waveLength;
                int y1 = (y0 + waveLength) % noiseHeight;
                float vertAlpha = (y - y0) * frequency;

                //interpolate the top corners with horizontal alpha/blend. Same thing with bottom corners
                //With the horizontal blend/alpha since:
                //                          .<-------->.
                //                          
                //                          .<-------->.

                float top = Lerp(baseNoise[x0,y0], baseNoise[x1, y0], horAlpha);                                //top
                float bottom = Lerp(baseNoise[x0, y1], baseNoise[x1, y1], horAlpha);                            //bottom

                //interpolate the two new values (the interpolated top corners - now one value - and the interpolated bottom corners - also now one value)...
                //...with the vertical blend/alpha since:
                //                          .
                //                          |
                //                          |
                //                          |
                //                          .
                smoothNoise[x,y] = Lerp(top, bottom, vertAlpha);
            }
        }
        return smoothNoise;
    }

    float Lerp(float a, float b, float alpha)
    {
        return a * (1 - alpha) + alpha * b;
    }

    //UI FUNCTIONS
    public void SetOctaveCount(float octaveCount)
    {
        octaveCounter = (int)octaveCount;
        //make the trees (if they exixt) follow the y-axis
        if (treesExist)
        {
            changeYValueOfTrees();
        }
        if (snowExists)
        {
            changeYValueOfSnow();
        }
    }

    public void SetAmplitude(float amp)
    {
        startAmplitude = amp;
        //make the trees (if they exixt) follow the y-axis
        if (treesExist)
        {
            changeYValueOfTrees();
        }
        if (snowExists)
        {
            changeYValueOfSnow();
        }
    }

    public void SetAmountOfTrees(float trees)
    {
        amountOfTrees = (int)trees;
        if(arrayFilled && treesExist)
        {
            ResetSnowOrTrees(false, treeArr, false, true);
            GrowTreesOnTerrain();
        }
    }

    public void SetAmountOfSnow(float snow)
    {
        amountOfSnow = (int)snow;
        if (snowArrFilled && snowExists)
        {
            ResetSnowOrTrees(false, snowArr, true, false);
            AddSnowToTerrain();
        }
    }

    public void SetPersistence(float p)
    {
        persistence = p;
        //make the trees (if they exixt) follow the y-axis and if they are out of bounce (outside of the constraints) the trees are "destroyed"
        if(treesExist)
        {
            changeYValueOfTrees();
        }
        if (snowExists)
        {
            changeYValueOfSnow();
        }
    }

    //sets the values of the height map for all elements such as snow, water, trees etc.
    public void SetSnowMinValue(InputField input)
    {
        float minVal = float.Parse(input.text);
        if (minVal >= 100 && minVal <= mapDepth)
        {
            snowMinVal = minVal;
            if(snowExists)
            {
                AddSnowToTerrain();
            }
        }
        else
        {
            ShowHiddenTextPanel();
        }

    }

    public void SetWaterMaxValue(InputField input)
    {
        float maxVal = float.Parse(input.text);
        if (maxVal > 0 && maxVal <= mapDepth)
        {
            waterMaxVal = maxVal;
            if(waterExists)
            {
                waterTransform.localPosition = new Vector3(waterTransform.localPosition.x, waterMaxVal, waterTransform.localPosition.z);
            }
        }
        else
        {
            ShowHiddenTextPanel();
        }
    }

    public void SetTreeMinValue(InputField input)
    {
        float minVal = float.Parse(input.text);
        if(minVal >= 0 && minVal <= mapDepth && minVal < treesMaxVal)
        {
            treesMinVal = minVal;
            if(treesExist)
            {
                GrowTreesOnTerrain();
            }
        }
        else
        {
            ShowHiddenTextPanel();
        }
    }

    public void SetTreeMaxValue(InputField input)
    {
        float maxVal = float.Parse(input.text);
        if (maxVal > treesMinVal && maxVal <= mapDepth)
        {
            treesMaxVal = maxVal;
            if(treesExist)
            {
                GrowTreesOnTerrain();
            }
        }
        else
        {
            ShowHiddenTextPanel();
        }
    }

    //shows the popup and closes it automatically (if not closed manually) after 5 seconds
    public void ShowHiddenTextPanel()
    {
        wrongInputImage.enabled = true;
        invalidInputText.enabled = true;
        invalidInputText2.enabled = true;
        closePopUpImageButton.gameObject.SetActive(true);
        StartCoroutine("wait");       
    }

    //used to close the popup after 5 seconds
    public IEnumerator wait()
    {
        yield return new WaitForSeconds(5);
        wrongInputImage.enabled = false;
        invalidInputText.enabled = false;
        invalidInputText2.enabled = false;
        closePopUpImageButton.gameObject.SetActive(false);
    }

    //shows the popup and closes it automatically (if not closed manually) after 5 seconds
    public void ShowHiddenSnowTextPanel()
    {
        wrongInputImageSnow.enabled = true;
        invalidInputTextSnow.enabled = true;
        invalidInputText2Snow.enabled = true;
        closePopUpImageButtonSnow.gameObject.SetActive(true);
        StartCoroutine("waitSnow");
    }

    //used to close the popup after 5 seconds
    public IEnumerator waitSnow()
    {
        yield return new WaitForSeconds(5);
        wrongInputImageSnow.enabled = false;
        invalidInputTextSnow.enabled = false;
        invalidInputText2Snow.enabled = false;
        closePopUpImageButtonSnow.gameObject.SetActive(false);
    }

    public IEnumerator waitAndPlotOutTrees()
    {
        yield return new WaitForSeconds(0.1f);
        GrowTreesOnTerrain();
    }

    public IEnumerator waitAndPlotOutSnow()
    {
        yield return new WaitForSeconds(0.1f);
        AddSnowToTerrain();
    }

    public void ClosePopUp()
    {
        wrongInputImage.enabled = false;
        invalidInputText.enabled = false;
        invalidInputText2.enabled = false;
        closePopUpImageButton.gameObject.SetActive(false);
    }

    public void CloseSnowPopUp()
    {
        wrongInputImageSnow.enabled = false;
        invalidInputTextSnow.enabled = false;
        invalidInputText2Snow.enabled = false;
        closePopUpImageButtonSnow.gameObject.SetActive(false);
    }

    public void SetSnowBool()
    {
        if (snowExists == false)
        {
            snowExists = true;
            SnowText.text = "Snow Added";
            AddSnowToTerrain();
        }
        else
        {
            snowExists = false;
            SnowText.text = "";
            ResetSnowOrTrees(false, snowArr, true, false);
        }
    }

    public void SetWaterBool()
    {
        if (waterExists == false)
        {
            waterExists = true;
            waterTransform.localPosition = new Vector3(waterTransform.localPosition.x, waterMaxVal, waterTransform.localPosition.z);
            WaterText.text = "Water Added";
        }
        else
        {
            waterExists = false;
            WaterText.text = "";
            waterTransform.localPosition = defaultWaterPos;
        }
    }

    public void SetTreeBool()
    {
        if(treesExist == false)
        {
            treesExist = true;
            TreeText.text = "Trees Added";
            GrowTreesOnTerrain();
        }
        else
        {
            treesExist = false;
            TreeText.text = "";
            ResetSnowOrTrees(false, treeArr, false, true);
        }
    }

    //resets the terrain and all values to the default-map
    public void resetTerrain()
    {
        snowExists = false;
        waterExists = false;
        treesExist = false;

        startAmplitude = defaultAmp;
        octaveCounter = defaultOct;
        persistence = defaultPer;

        noiseWidth = 513;
        noiseHeight = 513;
        mapDepth = 200;
        waterMaxVal = 20;
        GenerateTheTerrain(defaultTerrain, generatedBaseNoise);

        amplitudeSlider.value = startAmplitude;
        persistenceSlider.value = persistence;
        octaveSlider.value = octaveCounter;
        amountOfTreesSlider.value = amountOfTrees;

        waterTransform.localPosition = defaultWaterPos;

        WaterText.text = "";
        TreeText.text = "";
        SnowText.text = "";

        ResetSnowOrTrees(false, treeArr, false, true);
        ResetSnowOrTrees(false, snowArr, true, false);
    }

    public void GenerateNewRandomTerrain()
    {
        generatedBaseNoise = GenerateNoise(noiseWidth, noiseHeight);
        if(treesExist)
        {
            ResetSnowOrTrees(false, treeArr, false, true);
            StartCoroutine("waitAndPlotOutTrees");
        }
        if (snowExists)
        {
            ResetSnowOrTrees(false, snowArr, true, false);
            StartCoroutine("waitAndPlotOutSnow");
        }
        defaultAmp = startAmplitude;
        defaultOct = octaveCounter;
        defaultPer = persistence;
    }

    public void saveTerrain()
    {
        //saves = saves + 1;
        //string path = "Assets/Prefabs/SavedTerrain" + saves + ".prefab";
        //Object prefab = PrefabUtility.CreateEmptyPrefab(path);                                                  //create empty prefab object
        //PrefabUtility.ReplacePrefab(parentOfTerrain, prefab, ReplacePrefabOptions.ReplaceNameBased);            //"fill" the empty prefab with the selected object (in this case parentofterrain)
    }

    public void GrowTreesOnTerrain()
    {
        int checkIfAnyTreesAreAdded = 0;
        float currentHeight;
        float posx, posy, posz;
        //loop through the list of trees to set their position on the terrain
        for(int i=0; i<amountOfTrees; i++)
        {
            //loop until we find a point on the map where the height is within the constraints which the user has set
            //then we break and can finally set the position of this tree
            while(true)
            {
                //find a random point on the terrain and get the height (y-value) at that point
                //if it is within the constraints we can set posy and break the while loop.
                //if the highest value on the map is higher than our max-constraints we break
                posx = Random.Range(0, 513);
                posz = Random.Range(0, 513);
                currentHeight = Terrain.activeTerrain.SampleHeight(new Vector3(posx, 0, posz));
                float maxHeight = Terrain.activeTerrain.terrainData.bounds.max.y;
                if (currentHeight > treesMinVal && currentHeight < treesMaxVal)
                {
                    posy = currentHeight;
                    break;
                }
                else if (maxHeight < treesMaxVal)
                {
                    posy = -10;
                    break;
                }
            }
            if (posy == -10)
            {
                checkIfAnyTreesAreAdded++;
                if (checkIfAnyTreesAreAdded >= treeArr.Length)
                {
                    ShowHiddenSnowTextPanel();
                }
            }
            else
            {
                treeArr[i].transform.localPosition = new Vector3(posx, posy, posz);
            }
        }
    }

    public void AddSnowToTerrain()
    {
        int checkIfAnySnowIsAdded = 0;
        float currentHeight;
        float posx, posy, posz;
        for (int i = 0; i < snowArr.Length; i++)
        {
            while (true)
            {
                posx = Random.Range(0, 513);
                posz = Random.Range(0, 513);
                currentHeight = Terrain.activeTerrain.SampleHeight(new Vector3(posx, 0, posz));
                float maxHeight = Terrain.activeTerrain.terrainData.bounds.max.y;
                if (currentHeight >= snowMinVal && currentHeight <= mapDepth)
                {
                    posy = currentHeight;
                    break;
                }
                else if(maxHeight < mapDepth)
                {
                    posy = -10;
                    break;
                }
            }
            if(posy == -10)
            {
                checkIfAnySnowIsAdded++;
                if(checkIfAnySnowIsAdded >= snowArr.Length)
                {
                    ShowHiddenSnowTextPanel();
                }
            }
            else
            {
                snowArr[i].transform.localPosition = new Vector3(posx, posy, posz);
            }
        }
    }

    private void ResetSnowOrTrees(bool hideText, GameObject[] arr, bool snow, bool trees)
    {
        for (int i = 0; i < arr.Length; i++)
        {
            arr[i].transform.localPosition = defaultWaterPos;
        }

        if (hideText)
        {
            if(snow)
            {
                SnowText.text = "";
                snowExists = false;
            }
            if(trees)
            {
                TreeText.text = "";
                treesExist = false;
            }
        }
    }

    private void changeYValueOfTrees()
    {
        int tempLooper = amountOfTrees;
        float currentHeight;
        float posx, posy, posz;
        //loop through the list of trees to set their position on the terrain
        for (int i = 0; i < tempLooper; i++)
        {
            posx = treeArr[i].transform.position.x;
            posz = treeArr[i].transform.position.z;
            currentHeight = Terrain.activeTerrain.SampleHeight(new Vector3(posx, 0, posz));
            if (currentHeight > treesMinVal && currentHeight < treesMaxVal)
            {
                posy = currentHeight;
                treeArr[i].transform.localPosition = new Vector3(posx, posy, posz);
            }
            else
            {
                treeArr[i].transform.localPosition = defaultWaterPos;
                amountOfTrees = amountOfTrees - 1;
                amountOfTreesSlider.value = amountOfTrees;
                if(amountOfTrees<=0)
                {
                    TreeText.text = "";
                    treesExist = false;
                }
            } 
        }
    }

    private void changeYValueOfSnow()
    {
        float currentHeight;
        int countSnow = snowArr.Length;
        float posx, posy, posz;
        //loop through the list of trees to set their position on the terrain
        for (int i = 0; i < snowArr.Length; i++)
        {
            posx = snowArr[i].transform.position.x;
            posz = snowArr[i].transform.position.z;
            currentHeight = Terrain.activeTerrain.SampleHeight(new Vector3(posx, 0, posz));
            if (currentHeight >= snowMinVal && currentHeight <= mapDepth)
            {
                posy = currentHeight;
                snowArr[i].transform.localPosition = new Vector3(posx, posy, posz);
            }
            else
            {
                snowArr[i].transform.localPosition = defaultWaterPos;
                countSnow = countSnow - 1;
                if (countSnow <= 0)
                {
                    SnowText.text = "";
                    snowExists = false;
                }
            }
        }
    }
}